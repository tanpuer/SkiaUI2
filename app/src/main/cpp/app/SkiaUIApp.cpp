#include "SkiaUIApp.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "ViewGroup.h"
#include "ScrollView.h"
#include "core/SkGraphics.h"
#include "SkiaUIContext.h"
#include "CppTest.h"
#include "PageStackManager.h"
#include "JavascriptTest.h"
#include "JetpackComposeTest.h"

namespace HYSkiaUI {

SkiaUIApp::SkiaUIApp(JNIEnv *env, jobject javaAssetManager, jobject javaSkiaEngine,
                     int exampleType) {
    SkGraphics::Init();
    context = std::make_shared<SkiaUIContext>();
    context->setConfigRef(YGConfigNew());
    context->setJavaAssetManager(env, javaAssetManager);
    context->setJavaSkiaEngine(javaSkiaEngine);
    if (exampleType == 1) {
        testDraw = std::make_unique<JavascriptTest>();
    } else if (exampleType == 2) {
        testDraw = std::make_unique<JetpackComposeTest>(env);
    } else {
        testDraw = std::make_unique<CppTest>();
    }
    testDraw->setContext(context);
}

SkiaUIApp::~SkiaUIApp() {

}

long SkiaUIApp::doFrame(long time) {
    context->setTimeMills(time);
    IAnimator::currTime = time;
    if (!context->isDirty()) {
        return 0;
    }
    drawCount++;
    context->clearDirty();
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
    auto page = context->getPageStackManager()->back();
    if (page) {
        page->dispatchTouchEvent(mTouchEvent.get());
    }
}

void SkiaUIApp::setVelocity(Velocity *velocity) {
    auto page = context->getPageStackManager()->back();
    if (page) {
        page->dispatchVelocity(velocity);
    }
}

void SkiaUIApp::setWindowSize(int width, int height) {
    mWidth = width;
    mHeight = height;
}

bool SkiaUIApp::onBackPressed() {
    if (context->getPageStackManager()->getPages().size() <= 1) {
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
    context->markDirty();
}

void SkiaUIApp::onHide() {
    testDraw->onHide();
}

void SkiaUIApp::deleteSkPicture(long skPicture) {
    auto picture = reinterpret_cast<SkPicture *>(skPicture);
    if (picture != nullptr) {
        picture->unref();
    }
}

void SkiaUIApp::performTimeout(long id) {
    if (context) {
        context->performTimer(id);
    }
}

}
