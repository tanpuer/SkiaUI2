#pragma once

#include "jni.h"
#include "native_log.h"
#include "LottieView.h"

using namespace HYSkiaUI;

const char *HYComposeLottie = "com/temple/skiaui/compose/widget/HYComposeLottie";

extern "C" JNIEXPORT void JNICALL
compose_lottie_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto lottieView = reinterpret_cast<LottieView *>(viewPtr);
    if (lottieView == nullptr) {
        return;
    }
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    lottieView->setSource(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

extern "C" JNIEXPORT void JNICALL
compose_lottie_start(JNIEnv *env, jobject instance, jlong viewPtr) {
    auto lottieView = reinterpret_cast<LottieView *>(viewPtr);
    if (lottieView == nullptr) {
        return;
    }
    lottieView->start();
}

extern "C" JNIEXPORT void JNICALL
compose_lottie_pause(JNIEnv *env, jobject instance, jlong viewPtr) {
    auto lottieView = reinterpret_cast<LottieView *>(viewPtr);
    if (lottieView == nullptr) {
        return;
    }
    lottieView->pause();
}

static JNINativeMethod g_ComposeLottieViewMethods[] = {
        {"nativeSetSource", "(JLjava/lang/String;)V", (void *) compose_lottie_set_source},
        {"nativeStart",     "(J)V",                   (void *) compose_lottie_start},
        {"nativePause",     "(J)V",                   (void *) compose_lottie_pause},
};

static int RegisterComposeLottieMethods(JNIEnv *env) {
    ALOGD("RegisterComposeLottieMethods start %s", HYComposeLottie)
    jclass clazz = env->FindClass(HYComposeLottie);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeLottieMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeLottieViewMethods,
                             std::size(g_ComposeLottieViewMethods)) < 0) {
        ALOGD("RegisterComposeLottieMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeLottieMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeLottie);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeLottieMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}