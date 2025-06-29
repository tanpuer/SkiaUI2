#include "AndroidBitmap.h"
#include "bitmap_util.h"
#include "AndroidBitmapLoader.h"

namespace HYSkiaUI {

AndroidBitmap::AndroidBitmap(std::shared_ptr<SkiaUIContext> &context) {
    this->context = context;
}

AndroidBitmap::~AndroidBitmap() {
    if (javaInstance != nullptr) {
        auto jniEnv = context->getJniEnv();
        jniEnv->CallVoidMethod(javaInstance, releaseMethodId);
        jniEnv->DeleteGlobalRef(javaInstance);
        javaInstance = nullptr;
    }
}

void AndroidBitmap::setSource(const char *source) {
    this->resId = -1;
    this->source = source;
    checkInstance();
}

void AndroidBitmap::setResId(int resId) {
    this->source = "";
    this->resId = resId;
    checkInstance();
}

void AndroidBitmap::decode(int desiredWidth, int desiredHeight) {
    if (resId > 0) {
        auto jniEnv = context->getJniEnv();
        jniEnv->CallVoidMethod(javaInstance, setResIdMethodId, resId, desiredWidth, desiredHeight);
    } else if (!source.empty()) {
        auto jniEnv = context->getJniEnv();
        auto jSource = jniEnv->NewStringUTF(source.c_str());
        jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jSource, desiredWidth,
                               desiredHeight);
        jniEnv->DeleteLocalRef(jSource);
    }
}

void AndroidBitmap::start() {
    if (javaInstance == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(javaInstance, startMethodId);
}

void AndroidBitmap::stop() {
    if (javaInstance == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(javaInstance, stopMethodId);
}

int AndroidBitmap::setCallback(std::function<void(sk_sp<SkImage>, int, int)> &&callback) {
    if (callbackMap.empty()) {
        start();
    }
    auto callbackId = ID++;
    callbackMap[callbackId] = std::move(callback);
    return callbackId;
}

void AndroidBitmap::clearCallback(int callbackId) {
    callbackMap.erase(callbackId);
    if (callbackMap.empty()) {
        stop();
    }
}

void
HYSkiaUI::AndroidBitmap::setJavaBitmap(JNIEnv *env, jobject bitmap, int index, int frameCount) {
    if (javaInstance == nullptr) {
        return;
    }
    auto skImage = transferBitmapToSkImage(env, bitmap);
    for (auto& callback: callbackMap) {
        callback.second(skImage, index, frameCount);
    }
}

void AndroidBitmap::checkInstance() {
    if (javaInstance == nullptr) {
        auto jniEnv = context->getJniEnv();
        auto javaClass = jniEnv->FindClass("com/temple/skiaui/bitmap/AndroidBitmapLoader");
        auto javaConstructor = jniEnv->GetMethodID(javaClass, "<init>",
                                                   "(Lcom/temple/skiaui/HYSkiaEngine;J)V");
        auto javaSkiaEngine = context->getJavaSkiaEngine();
        javaInstance = jniEnv->NewGlobalRef(
                jniEnv->NewObject(javaClass, javaConstructor, javaSkiaEngine,
                                  reinterpret_cast<long>(this)));
        setSourceMethodId = jniEnv->GetMethodID(javaClass, "setSource", "(Ljava/lang/String;II)V");
        releaseMethodId = jniEnv->GetMethodID(javaClass, "release", "()V");
        setResIdMethodId = jniEnv->GetMethodID(javaClass, "setResId", "(III)V");
        startMethodId = jniEnv->GetMethodID(javaClass, "start", "()V");
        stopMethodId = jniEnv->GetMethodID(javaClass, "stop", "()V");
    }
}

void AndroidBitmap::ref() {
    refCount++;
}

void AndroidBitmap::unRef() {
    refCount--;
    if (refCount == 0) {
        auto url = source;
        if (url.empty()) {
            url = std::to_string(resId);
        }
        AndroidBitmapLoader::getInstance()->clear(url);
    }
}

}
