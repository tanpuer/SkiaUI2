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
#include "gpu/ganesh/gl/GrGLBackendSurface.h"
#include "gpu/GrBackendSurface.h"
#include "android/GrAHardwareBufferUtils.h"
#include "gpu/ganesh/gl/GrGLDefines.h"

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
    GrBackendFormat format;
    switch (description.format) {
        // TODO: find out if we can detect, which graphic buffers support
        // GR_GL_TEXTURE_2D
        case AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM:
            format = GrBackendFormats::MakeGL(GR_GL_RGBA8, GR_GL_TEXTURE_EXTERNAL);
        case AHARDWAREBUFFER_FORMAT_R16G16B16A16_FLOAT:
            format = GrBackendFormats::MakeGL(GR_GL_RGBA16F, GR_GL_TEXTURE_EXTERNAL);
        case AHARDWAREBUFFER_FORMAT_R5G6B5_UNORM:
            format = GrBackendFormats::MakeGL(GR_GL_RGB565, GR_GL_TEXTURE_EXTERNAL);
        case AHARDWAREBUFFER_FORMAT_R10G10B10A2_UNORM:
            format = GrBackendFormats::MakeGL(GR_GL_RGB10_A2, GR_GL_TEXTURE_EXTERNAL);
        case AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM:
            format = GrBackendFormats::MakeGL(GR_GL_RGB8, GR_GL_TEXTURE_EXTERNAL);
#if __ANDROID_API__ >= 33
            case AHARDWAREBUFFER_FORMAT_R8_UNORM:
    format = GrBackendFormats::MakeGL(GR_GL_R8, GR_GL_TEXTURE_EXTERNAL);
#endif
        default:
            format = GrBackendFormats::MakeGL(GR_GL_RGB8, GR_GL_TEXTURE_EXTERNAL);
    }
    return 0L;
//    auto backendTex = GrAHardwareBufferUtils::MakeGLBackendTexture(
//            skiaContext.get(),
//            const_cast<AHardwareBuffer *>(hardwareBuffer), description.width,
//            description.height, &deleteImageProc, &updateImageProc, &deleteImageCtx,
//            false, format, false);
//    if (!backendTex.isValid()) {
//        ALOGE("Failed to convert HardwareBuffer to OpenGL Texture!")
//    }
//    sk_sp<SkImage> image = SkImages::BorrowTextureFrom(
//            skiaContext.get(),
//            backendTex, kTopLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType,
//            kOpaque_SkAlphaType, nullptr, deleteImageProc, deleteImageCtx);
//    image->ref();
//    return reinterpret_cast<long >(image.get());
}
