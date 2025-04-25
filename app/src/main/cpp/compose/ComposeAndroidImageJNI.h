#pragma once

#include "jni.h"
#include "native_log.h"
#include "AndroidImageView.h"

using namespace HYSkiaUI;

const char *HYComposeAndroidImage = "com/temple/skiaui/compose/ui/HYComposeAndroidImage";

extern "C" JNIEXPORT void JNICALL
compose_android_image_set_resource(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto imageView = reinterpret_cast<AndroidImageView *>(viewPtr);
    if (imageView == nullptr) {
        return;
    }
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    imageView->setSource(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

extern "C" JNIEXPORT void JNICALL
compose_android_image_set_resid(JNIEnv *env, jobject instance, jlong viewPtr, jint resId) {
    auto imageView = reinterpret_cast<AndroidImageView *>(viewPtr);
    if (imageView == nullptr) {
        return;
    }
    imageView->setResId(resId);
}

extern "C" JNIEXPORT void JNICALL
compose_android_image_set_scale(JNIEnv *env, jobject instance, jlong viewPtr, jstring scale) {
    auto imageView = reinterpret_cast<AndroidImageView *>(viewPtr);
    if (imageView == nullptr) {
        return;
    }
    auto scaleStr = env->GetStringUTFChars(scale, nullptr);
    imageView->setScaleType(W3CToScaleType(scaleStr));
    env->ReleaseStringUTFChars(scale, scaleStr);
}

extern "C" JNIEXPORT void JNICALL
compose_android_image_set_blur(JNIEnv *env, jobject instance, jlong viewPtr, jfloat blur) {
    auto imageView = reinterpret_cast<AndroidImageView *>(viewPtr);
    if (imageView == nullptr) {
        return;
    }
    imageView->blur(blur);
}

static JNINativeMethod g_ComposeAndroidImageViewMethods[] = {
        {"nativeSetResource", "(JLjava/lang/String;)V", (void *) compose_android_image_set_resource},
        {"nativeSetResId",    "(JI)V",                  (void *) compose_android_image_set_resid},
        {"nativeSetScale",    "(JLjava/lang/String;)V", (void *) compose_android_image_set_scale},
        {"nativeSetBlur",     "(JF)V",                  (void *) compose_android_image_set_blur},
};

static int RegisterComposeAndroidImageMethods(JNIEnv *env) {
    ALOGD("RegisterComposeImageMethods start %s", HYComposeAndroidImage)
    jclass clazz = env->FindClass(HYComposeAndroidImage);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeAndroidImageMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeAndroidImageViewMethods,
                             std::size(g_ComposeAndroidImageViewMethods)) < 0) {
        ALOGD("RegisterComposeAndroidImageMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeAndroidImageMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeAndroidImage);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeAndroidImageMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}