#include <GLES3/gl3.h>
#include <base/native_log.h>
#include "SkiaFilter.h"
#include "core/SkGraphics.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "gpu/ganesh/gl/GrGLDirectContext.h"
#include "gpu/GrBackendSurface.h"
#include "gpu/ganesh/SkSurfaceGanesh.h"
#include "core/SkColorSpace.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include <android/hardware_buffer.h>
#include <android/hardware_buffer_jni.h>
#include "gpu/ganesh/SkImageGanesh.h"
#include "GrAHardwareBufferUtils.h"
#include "android/GrAHardwareBufferUtils.h"

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
    SkASSERT(skCanvas);
    skCanvas->clear(SK_ColorWHITE);
    picture->playback(skCanvas);
    picture->unref();
    skiaContext->flush();
}

long SkiaFilter::MakeHardwareBufferToSkImage(JNIEnv *env, jobject javaHardwareBuffer) {
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
        while (skImage->getRefCnt2()) {
            skImage->unref();
        }
    }
}
