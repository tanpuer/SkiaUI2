#include "SkiaGLApp.h"
#include "SkiaFilter.h"

SkiaGLApp::SkiaGLApp(JNIEnv *env) {
}

SkiaGLApp::~SkiaGLApp() {

}

void SkiaGLApp::create(ANativeWindow *window) {
    mEGLCore = std::make_unique<EGLCore>();
    mEGLCore->createGLEnv(nullptr, window, 0, 0, false);
    mEGLCore->makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    mFilter = std::make_unique<SkiaFilter>();
}

void SkiaGLApp::change(int width, int height, long time) {
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, width, height);
    mFilter->setWindowSize(width, height);
    mEGLCore->swapBuffer();
}

void SkiaGLApp::destroy() {
    mFilter.reset(nullptr);
    mEGLCore.reset(nullptr);
}

void SkiaGLApp::doFrame(long pic, long time) {
    if (mEGLCore == nullptr || mFilter == nullptr) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    auto skPicture = reinterpret_cast<SkPicture *>(pic);
    assert(skPicture != nullptr);
    mFilter->render(skPicture);
    mEGLCore->swapBuffer();
}

long SkiaGLApp::MakeHardwareBufferToSkImage(JNIEnv *env, jobject hardwareBuffer) {
    if (mFilter != nullptr) {
        return mFilter->MakeHardwareBufferToSkImage(env, hardwareBuffer);
    }
    return 0;
}

void SkiaGLApp::deleteSkImage(JNIEnv *env, long skImagePtr) {
    if (mFilter != nullptr) {
        mFilter->deleteSkImage(env, skImagePtr);
    }
}
