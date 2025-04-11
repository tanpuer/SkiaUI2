#pragma once

#include "jni.h"
#include "Page.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeScrollView = "com/temple/skiaui/compose/ui/HYComposeScrollView";

static JNINativeMethod g_ComposeScrollViewMethods[] = {
};

static int RegisterComposeScrollMethods(JNIEnv *env) {
    ALOGD("RegisterComposeScrollMethods start %s", HYComposeScrollView)
//    jclass clazz = env->FindClass(HYComposePage);
//    if (clazz == nullptr) {
//        ALOGD("RegisterComposePageMethods fail clazz == null")
//        return JNI_FALSE;
//    }
//    if (env->RegisterNatives(clazz, g_ComposePageMethods, std::size(g_ComposePageMethods)) < 0) {
//        ALOGD("RegisterComposePageMethods fail")
//        return JNI_FALSE;
//    }
    return JNI_TRUE;
}

static void UnRegisterComposeScrollMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeScrollView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeScrollMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
