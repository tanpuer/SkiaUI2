#include <GLES3/gl3.h>
#include <base/native_log.h>
#include "SkiaFilter.h"
#include "core/SkGraphics.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "gpu/ganesh/gl/GrGLDirectContext.h"
#include "gpu/ganesh/GrBackendSurface.h"
#include "gpu/ganesh/SkSurfaceGanesh.h"
#include "core/SkColorSpace.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include <android/hardware_buffer.h>
#include <android/hardware_buffer_jni.h>
#include "gpu/ganesh/SkImageGanesh.h"
#include "GrAHardwareBufferUtils.h"
#include "android/GrAHardwareBufferUtils.h"
#include "gpu/ganesh/gl/GrGLDefines.h"

namespace HYSkiaUI {

SkiaFilter::SkiaFilter() : skCanvas(nullptr) {
    SkGraphics::Init();
}

SkiaFilter::~SkiaFilter() {
    skCanvas = nullptr;
}

void SkiaFilter::setWindowSize(int width, int height) {
    this->width = width;
    this->height = height;
    auto backendInterface = GrGLMakeNativeInterface();
    skiaContext = GrDirectContexts::MakeGL(backendInterface);
    SkASSERT(skiaContext);
    GLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    GLint stencil;
    glGetIntegerv(GL_STENCIL_BITS, &stencil);
    GLint samples;
    glGetIntegerv(GL_SAMPLES, &samples);
    auto maxSamples = skiaContext->maxSurfaceSampleCountForColorType(kRGBA_8888_SkColorType);
    if (samples > maxSamples)
        samples = maxSamples;
    GrGLFramebufferInfo fbInfo;
    fbInfo.fFBOID = buffer;
    fbInfo.fFormat = GL_RGBA8;
    auto _skRenderTarget = GrBackendRenderTargets::MakeGL(width, height, samples,
                                                          stencil, fbInfo);
    skiaSurface = SkSurfaces::WrapBackendRenderTarget(
            skiaContext.get(),
            _skRenderTarget,
            kBottomLeft_GrSurfaceOrigin,
            kRGBA_8888_SkColorType,
            nullptr,
            nullptr);

    SkASSERT(skiaSurface);
    skCanvas = skiaSurface->getCanvas();
}

void SkiaFilter::render(SkPicture *picture) {
//    MeasureTime measureTime("SkiaFilter::render");
    SkASSERT(skCanvas);
    skCanvas->clear(SK_ColorWHITE);
    picture->playback(skCanvas);
    picture->unref();
    skiaContext->flush();
}

long SkiaFilter::MakeHardwareBufferToSkImage(JNIEnv *env, jobject javaHardwareBuffer) {
    MeasureTime measureTime("MakeHardwareBufferToSkImage");
    AHardwareBuffer *buffer = AHardwareBuffer_fromHardwareBuffer(env, javaHardwareBuffer);
    // Setup OpenGL and Skia:
    const AHardwareBuffer *hardwareBuffer = static_cast<AHardwareBuffer *>(buffer);
    GrAHardwareBufferUtils::DeleteImageProc deleteImageProc = nullptr;
    GrAHardwareBufferUtils::UpdateImageProc updateImageProc = nullptr;
    GrAHardwareBufferUtils::TexImageCtx deleteImageCtx = nullptr;

    AHardwareBuffer_Desc description;
    AHardwareBuffer_describe(hardwareBuffer, &description);
    GrBackendFormat format = GetGLBackendFormat(skiaContext.get(), description.format, false);
    auto backendTex = MakeGLBackendTexture(
            skiaContext.get(),
            const_cast<AHardwareBuffer *>(hardwareBuffer), description.width,
            description.height, &deleteImageProc, &updateImageProc, &deleteImageCtx,
            false, format, false);
    if (!backendTex.isValid()) {
        ALOGE("Failed to convert HardwareBuffer to OpenGL Texture!")
        return 0L;
    }
    sk_sp<SkImage> image = SkImages::BorrowTextureFrom(
            skiaContext.get(),
            backendTex, kTopLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType,
            kOpaque_SkAlphaType, nullptr, deleteImageProc, deleteImageCtx);
    image->ref();
    return reinterpret_cast<long >(image.get());
}

void SkiaFilter::deleteSkImage(JNIEnv *env, long skImagePtr) {
    auto skImage = reinterpret_cast<SkImage *>(skImagePtr);
    if (skImage != nullptr) {
        ALOGD("textureId is deleteSkImage: refCount: %d", skImage->getRefCnt2())
        SkSafeUnref(skImage);
    }
}

long SkiaFilter::attachSurfaceTexture(JNIEnv *env, int width, int height, jobject surfaceTexture) {
    GrGLuint texID;
    glGenTextures(1, &texID);

    GrGLuint target = GR_GL_TEXTURE_EXTERNAL;
    glBindTexture(target, texID);
    // 设置纹理参数
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum status = glGetError();
    if (status != GL_NO_ERROR) {
        ALOGE("OpenGL error after binding texture: %d", status);
        return 0L;
    }

    static auto jClazz = env->GetObjectClass(surfaceTexture);
    static auto attachToGLContext = env->GetMethodID(jClazz, "attachToGLContext", "(I)V");
    static auto updateTexImage = env->GetMethodID(jClazz, "updateTexImage", "()V");

    env->CallVoidMethod(surfaceTexture, attachToGLContext, texID);
    env->CallVoidMethod(surfaceTexture, updateTexImage);

    GrBackendFormat format = GetGLBackendFormat(skiaContext.get(), GR_GL_RGBA8, false);
    GrGLTextureInfo textureInfo;
    textureInfo.fID = texID;
    textureInfo.fTarget = target;
    textureInfo.fFormat = GrBackendFormats::AsGLFormatEnum(format);
    textureInfo.fProtected = skgpu::Protected(false);

    auto backendTex = GrBackendTextures::MakeGL(width, height, skgpu::Mipmapped::kNo, textureInfo);
    if (!backendTex.isValid()) {
        ALOGE("Failed to convert surfaceTexture to OpenGL Texture!")
        return 0L;
    }
    auto textureSurfaceHelper = new TextureSurfaceHelper(texID);
    sk_sp<SkImage> image = SkImages::BorrowTextureFrom(
            skiaContext.get(),
            backendTex, kTopLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType,
            kOpaque_SkAlphaType, nullptr, [](void *ctx) {
                auto helper = static_cast<TextureSurfaceHelper *>(ctx);
                delete helper;
            }, textureSurfaceHelper);
    image->ref();
    return reinterpret_cast<long >(image.get());
}

}
