//
// Created by cw on 1/21/22.
//

#include <GLES3/gl3.h>
#include <base/native_log.h>
#include <yoga/Yoga.h>
#include <ScrollViewTest.h>
#include <ScrollView.h>
#include <ProgressBarDrawTest.h>
#include <ListViewTest.h>
#include <BaseListView.h>
#include "SkiaFilter.h"
#include "core/SkGraphics.h"
#include "HorizontalDrawTest.h"
#include "VerticalDrawTest.h"
#include "MovingDrawTest.h"
#include "IAnimator.h"
#include "ImageViewTest.h"
#include "TextViewTest.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "gpu/ganesh/gl/GrGLDirectContext.h"
#include "gpu/GrBackendSurface.h"
#include "gpu/ganesh/SkSurfaceGanesh.h"
#include "core/SkColorSpace.h"


SkiaFilter::SkiaFilter() : skCanvas(nullptr) {
    SkGraphics::Init();
//    testDraw = new HorizontalDrawTest();
//    testDraw = new ImageViewTest();
//    testDraw = new TextViewTest();
//    testDraw = new MovingDrawTest();
    testDraw = new ScrollViewTest();
//    testDraw = new ProgressBarDrawTest();
//    testDraw = new ListViewTest();
}

SkiaFilter::~SkiaFilter() {
    if (skiaSurface != nullptr) {
        skiaSurface->unref();
    }
    if (skiaContext != nullptr) {
        skiaContext->unref();
    }
    delete testDraw;
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

void SkiaFilter::doFrame(long time) {
    IFilter::doFrame(time);
    IAnimator::currTime = time;
    SkASSERT(skCanvas);
    skCanvas->clear(SK_ColorWHITE);
    testDraw->doDrawTest(drawCount, skCanvas, width, height);
    skiaContext->flush();
}

void SkiaFilter::dispatchTouchEvent(TouchEvent *touchEvent) {
    mTouchEvent = std::unique_ptr<TouchEvent>(touchEvent);
    auto root = testDraw->getRootView();
    if (root == nullptr) {
        return;
    }
    dynamic_cast<ViewGroup *>(root)->dispatchTouchEvent(mTouchEvent.get());
}

void SkiaFilter::setVelocity(Velocity *velocity) {
    auto root = testDraw->getRootView();
    auto scrollView = dynamic_cast<ScrollView *>(root);
    if (scrollView != nullptr) {
        scrollView->setVelocity(velocity->xVelocity, velocity->yVelocity);
        return;
    }
    auto listView = dynamic_cast<BaseListView<void *> *>(root);
    if (listView != nullptr) {
        listView->setVelocity(velocity->xVelocity, velocity->yVelocity);
    }
}
