#include "SkiaUIApp.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "ViewGroup.h"
#include "ScrollView.h"
#include "core/SkGraphics.h"
#include "SkiaUIContext.h"
#include "PageTest.h"
#include "PageStackManager.h"
#include "FlexboxLayoutTest.h"
#include "JavascriptTest.h"

SkiaUIApp::SkiaUIApp(JNIEnv *env, jobject javaAssetManager, jobject javaSkiaEngine) {
    SkGraphics::Init();
    context = std::make_shared<SkiaUIContext>();
    context->setJavaAssetManager(env, javaAssetManager);
    context->setJavaSkiaEngine(javaSkiaEngine);
    testDraw = std::make_unique<PageTest>();
//    testDraw = std::make_unique<FlexboxLayoutTest>();
//    testDraw = std::make_unique<JavascriptTest>();
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
    context->setCanvas(recordingCanvas);
    testDraw->doDrawTest(drawCount, recordingCanvas, mWidth, mHeight);
    context->setCanvas(nullptr);
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
}

void SkiaUIApp::setWindowSize(int width, int height) {
    mWidth = width;
    mHeight = height;
}

bool SkiaUIApp::onBackPressed() {
    if (context->getPageStackManager()->getPages().size() <=1) {
        context->getPluginManager()->invokeMethod("toast", "show", "back error");
        return false;
    }
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

SkiaUIContext *SkiaUIApp::getContext() {
    return context.get();
}

void SkiaUIApp::executeTask(JNIEnv *env, int taskId, jobject javaAssets) {
    if (context->resourcesLoader) {
        context->resourcesLoader->executeTask(env, taskId, javaAssets);
    }
}

void SkiaUIApp::postTask(JNIEnv *env, int taskId) {
    if (context->resourcesLoader) {
        context->resourcesLoader->postTask(env, taskId);
    }
}

void SkiaUIApp::onShow() {
    testDraw->onShow();
}

void SkiaUIApp::onHide() {
    testDraw->onHide();
}
