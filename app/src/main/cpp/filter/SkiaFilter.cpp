//
// Created by cw on 1/21/22.
//

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

long SkiaFilter::MakeHardwareBufferToSkImage(JNIEnv *env, jobject hardwareBuffer) {
    return 0;
}
