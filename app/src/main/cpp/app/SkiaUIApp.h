#pragma once

#include "TouchEvent.h"
#include "memory"
#include "ITestDraw.h"
#include "Velocity.h"
#include "PluginManager.h"

/**
 * UI线程执行的逻辑入口
 */
class SkiaUIApp {

public:

    SkiaUIApp(JNIEnv *env, jobject javaAssetManager, jobject javaSkiaEngine);

    ~SkiaUIApp();

    void setWindowSize(int width, int height);

    void dispatchTouchEvent(TouchEvent *touchEvent);

    void setVelocity(Velocity *velocity);

    long doFrame(long time);

    bool onBackPressed();

    void initJavaPluginManager(JNIEnv *env, jobject javaPlugins);

    void releaseJavaPluginManager(JNIEnv *env);

    SkiaUIContext* getContext();

private:

    std::unique_ptr<TouchEvent> mTouchEvent;

    std::unique_ptr<ITestDraw> testDraw;

    int mWidth = 0, mHeight = 0;

    int drawCount = 0;

    std::shared_ptr<SkiaUIContext> context = nullptr;

};