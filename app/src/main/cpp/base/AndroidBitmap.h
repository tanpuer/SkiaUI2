#pragma once

#include "core/SkImage.h"
#include "memory"
#include "functional"
#include "jni.h"
#include "SkiaUIContext.h"

namespace HYSkiaUI {

class AndroidBitmap {

public:

    explicit AndroidBitmap(std::shared_ptr<SkiaUIContext> &context);

    ~AndroidBitmap();

    void setSource(const char *source);

    void setResId(int resId);

    void decode(int desiredWidth, int desiredHeight);

    void start();

    void stop();

    void setCallback(std::function<void(sk_sp<SkImage>, int, int)> &&callback);

    void setJavaBitmap(JNIEnv *env, jobject bitmap, int index, int frameCount);

    void checkInstance();

private:

    jobject javaInstance = nullptr;

    jmethodID setSourceMethodId = nullptr;

    jmethodID releaseMethodId = nullptr;

    jmethodID setResIdMethodId = nullptr;

    jmethodID startMethodId = nullptr;

    jmethodID stopMethodId = nullptr;

    std::function<void(sk_sp<SkImage>, int, int)> callback = nullptr;

    std::shared_ptr<SkiaUIContext> context = nullptr;

    std::string source;

    int resId = -1;

};

}
