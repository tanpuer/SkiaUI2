#pragma once

#include "jni.h"
#include "LoadingView.h"
#include "native_log.h"
#include "color_util.h"

using namespace HYSkiaUI;

const char *HYComposeLoadingView = "com/temple/skiaui/compose/widget/HYComposeLoadingView";

extern "C" JNIEXPORT void JNICALL
compose_loading_set_color(JNIEnv *env, jobject instance, jlong viewPtr, jstring color) {
    auto lottieView = reinterpret_cast<LoadingView *>(viewPtr);
    auto colorStr = env->GetStringUTFChars(color, nullptr);
    int r, g, b, a;
    hexToRGBA(colorStr, r, g, b, a);
    lottieView->setLoadingColor(SkColorSetARGB(a, r, g, b));
    env->ReleaseStringUTFChars(color, colorStr);
}

static JNINativeMethod g_ComposeLoadingViewMethods[] = {
        {"nativeSetColor", "(JLjava/lang/String;)V", (void *) compose_loading_set_color},
};

static int RegisterComposeLoadingViewMethods(JNIEnv *env) {
    ALOGD("RegisterComposeLoadingViewMethods start %s", HYComposeLoadingView)
    jclass clazz = env->FindClass(HYComposeLoadingView);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeLoadingViewMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeLoadingViewMethods,
                             std::size(g_ComposeLoadingViewMethods)) < 0) {
        ALOGD("RegisterComposeLoadingViewMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeLoadingViewMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeLoadingView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeLoadingViewMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
