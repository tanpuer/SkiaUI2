#pragma once

#include "jni.h"
#include "jni.h"
#include "EGLCore.h"
#include "memory"
#include "AssetManager.h"
#include "IFilter.h"

namespace HYSkiaUI {

class SkiaGLApp {

public:

    explicit SkiaGLApp(JNIEnv *env);

    ~SkiaGLApp();

    void create(ANativeWindow *window);

    void change(int width, int height, long time);

    void destroy();

    void destroyInMainThread();

    void doFrame(long pic, long time);

    long MakeHardwareBufferToSkImage(JNIEnv *env, jobject hardwareBuffer);

    void deleteSkImage(JNIEnv *env, long skImagePtr);

    long attachSurfaceTexture(JNIEnv* env, int width, int height, jobject surfaceTexture);

    void updateTexImage(JNIEnv* env, jobject surfaceTexture, long skImagePtr);

private:

    std::unique_ptr<EGLCore> mEGLCore;
    std::unique_ptr<IFilter> mFilter;
    int mWidth = 0, mHeight = 0;
    JNIEnv *env = nullptr;
    std::atomic<bool> isSurfaceValid {false};
};

}
