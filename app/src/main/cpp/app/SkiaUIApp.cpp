//
// Created by banma-3412 on 2024/2/12.
//

#include "SkiaUIApp.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "ViewGroup.h"
#include "ScrollView.h"
#include "BaseListView.h"
#include "core/SkGraphics.h"
#include "SkiaUIContext.h"
#include "PageTest.h"
#include "PageStackManager.h"
#include "FlexboxLayoutTest.h"

SkiaUIApp::SkiaUIApp(JNIEnv *env, jobject javaAssetManager) {
    SkGraphics::Init();
    context = std::make_shared<SkiaUIContext>();
    context->setJavaAssetManager(env, javaAssetManager);
    testDraw = std::make_unique<PageTest>();
//    testDraw = std::make_unique<FlexboxLayoutTest>();
    testDraw->setContext(context);
}

SkiaUIApp::~SkiaUIApp() {

}

long SkiaUIApp::doFrame(long time) {
    context->setTimeMills(time);
    drawCount++;
    IAnimator::currTime = time;
    SkPictureRecorder recorder;
    auto recordingCanvas = recorder.beginRecording(mWidth, mHeight);
    testDraw->doDrawTest(drawCount, recordingCanvas, mWidth, mHeight);
    auto picture = recorder.finishRecordingAsPicture();
    picture->ref();
    return reinterpret_cast<long >(picture.get());
}

void SkiaUIApp::dispatchTouchEvent(TouchEvent *touchEvent) {
    mTouchEvent = std::unique_ptr<TouchEvent>(touchEvent);
    auto root = testDraw->getRootView();
    if (root == nullptr) {
        return;
    }
    dynamic_cast<ViewGroup *>(root)->dispatchTouchEvent(mTouchEvent.get());
}

void SkiaUIApp::setVelocity(Velocity *velocity) {
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

void SkiaUIApp::setWindowSize(int width, int height) {
    mWidth = width;
    mHeight = height;
}

bool SkiaUIApp::onBackPressed() {
    auto page = context->getPageStackManager()->back();
    if (page != nullptr) {
        page->exitToLeft(Page::EnterExitInfo(0, mWidth));
        return true;
    }
    return false;
}

void SkiaUIApp::initJavaPluginManager(JNIEnv *env, jobject javaPlugins) {
    context->getPluginManager()->initJavaPluginManager(javaPlugins, env);
}

void SkiaUIApp::releaseJavaPluginManager(JNIEnv *env) {
    context->getPluginManager()->releaseJavaPluginManager(env);
}
