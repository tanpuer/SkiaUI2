#pragma once

#include "jni.h"
#include "LoadingView.h"
#include "native_log.h"

const char *HYComposeLoadingView = "com/temple/skiaui/compose/widget/HYComposeLoadingView";


static JNINativeMethod g_ComposeLoadingViewMethods[] = {};

static int RegisterComposeLoadingViewMethods(JNIEnv *env) {
    ALOGD("RegisterComposeLoadingViewMethods start %s", HYComposeLoadingView)
//    jclass clazz = env->FindClass(HYComposeLoadingView);
//    if (clazz == nullptr) {
//        ALOGD("RegisterComposeLoadingViewMethods fail clazz == null")
//        return JNI_FALSE;
//    }
//    if (env->RegisterNatives(clazz, g_ComposeLoadingViewMethods,
//                             std::size(g_ComposeLoadingViewMethods)) < 0) {
//        ALOGD("RegisterComposeLoadingViewMethods fail")
//        return JNI_FALSE;
//    }
    return JNI_TRUE;
}

static void UnRegisterComposeLoadingViewMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeLoadingView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeLoadingViewMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
