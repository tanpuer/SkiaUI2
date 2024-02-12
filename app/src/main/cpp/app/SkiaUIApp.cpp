//
// Created by cw on 2024/2/4.
//

#include "SkiaUIApp.h"
#include "SkiaFilter.h"
#include "SkiaUIContext.h"

SkiaUIApp::SkiaUIApp(JNIEnv *env, jobject javaAssetManager) {
    SkiaUIContext::getInstance()->setJavaAssetManager(env, javaAssetManager);
}

SkiaUIApp::~SkiaUIApp() {

}

void SkiaUIApp::create(ANativeWindow *window) {
    mEGLCore = std::make_unique<EGLCore>();
    mEGLCore->createGLEnv(nullptr, window, 0, 0, false);
    mEGLCore->makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    mFilter = std::make_unique<SkiaFilter>();
    recorder = std::make_unique<SkPictureRecorder>();
}

void SkiaUIApp::change(int width, int height, long time) {
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, width, height);
    mFilter->setWindowSize(width, height);
    mFilter->doFrame(time);
    mEGLCore->swapBuffer();
}

void SkiaUIApp::destroy() {
    mFilter.reset(nullptr);
    mEGLCore.reset(nullptr);
}

void SkiaUIApp::doFrame(long time) {
    if (mEGLCore == nullptr || mFilter == nullptr) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    mFilter->doFrame(time);
    mEGLCore->swapBuffer();
}

void SkiaUIApp::dispatchTouchEvent(TouchEvent *touchEvent) {
    mFilter->dispatchTouchEvent(touchEvent);
}

void SkiaUIApp::setVelocity(float x, float y) {
    mFilter->setVelocity(new Velocity(x, y));
}
