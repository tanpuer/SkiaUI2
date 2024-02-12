//
// Created by cw on 2024/2/4.
//

#ifndef SKIAUI_SKIAUIAPP_H
#define SKIAUI_SKIAUIAPP_H

#include "jni.h"
#include "jni.h"
#include "EGLCore.h"
#include "memory"
#include "AssetManager.h"
#include "IFilter.h"
#include "core/SkPictureRecorder.h"

class SkiaUIApp {

public:

    SkiaUIApp(JNIEnv *env, jobject javaAssetManager);

    ~SkiaUIApp();

    void create(ANativeWindow *window);

    void change(int width, int height, long time);

    void destroy();

    void doFrame(long time);

    void dispatchTouchEvent(TouchEvent *touchEvent);

    void setVelocity(float x, float y);

private:

    std::unique_ptr<EGLCore> mEGLCore;
    std::unique_ptr<IFilter> mFilter;
    int mWidth = 0, mHeight = 0;

    std::unique_ptr<SkPictureRecorder> recorder;

};


#endif //SKIAUI_SKIAUIAPP_H
