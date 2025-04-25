#pragma once

#include "jni.h"
#include "native_log.h"
#include "ProgressBar.h"

using namespace HYSkiaUI;

const char *HYComposeProgressBar = "com/temple/skiaui/compose/ui/HYComposeProgressBar";

extern "C" JNIEXPORT void JNICALL
compose_progress_set_bar_color(JNIEnv *env, jobject instance, jlong viewPtr, jint color) {
    auto progressBar = reinterpret_cast<ProgressBar *>(viewPtr);
    if (progressBar != nullptr) {
        progressBar->setBarColor(color);
    }
}

extern "C" JNIEXPORT void JNICALL
compose_progress_set_change_callback(JNIEnv *env, jobject instance, jlong viewPtr) {
    auto progressBar = reinterpret_cast<ProgressBar *>(viewPtr);
    if (progressBar != nullptr) {
        progressBar->setProgressCallback([progressBar](int progress, bool finished) {
            auto jniEnv = progressBar->getContext()->getJniEnv();
            auto javaInstance = progressBar->getJavaViewRef();
            static jmethodID onChangeFromNativeMethodId = jniEnv->GetMethodID(
                    jniEnv->GetObjectClass(javaInstance), "onChangeFromNative", "(I)V");
            jniEnv->CallVoidMethod(javaInstance, onChangeFromNativeMethodId, progress);
        });
    }
}

extern "C" JNIEXPORT void JNICALL
compose_progress_set_progress(JNIEnv *env, jobject instance, jlong viewPtr, jint progress) {
    auto progressBar = reinterpret_cast<ProgressBar *>(viewPtr);
    if (progressBar != nullptr) {
        progressBar->setProgress(progress);
    }
}

static JNINativeMethod g_ComposeProgressBarMethods[] = {
        {"nativeSetBarColor",         "(JI)V", (void *) compose_progress_set_bar_color},
        {"nativeSetOnChangeCallback", "(J)V",  (void *) compose_progress_set_change_callback},
        {"nativeSetProgress",         "(JI)V", (void *) compose_progress_set_progress},
};

static int RegisterComposeProgressMethods(JNIEnv *env) {
    ALOGD("RegisterComposeProgressMethods start %s", HYComposeProgressBar)
    jclass clazz = env->FindClass(HYComposeProgressBar);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeProgressMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeProgressBarMethods,
                             std::size(g_ComposeProgressBarMethods)) < 0) {
        ALOGD("RegisterComposeProgressMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeProgressMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeProgressBar);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeProgressMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}