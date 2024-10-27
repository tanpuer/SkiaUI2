#pragma once

#include "jni.h"
#include "native_log.h"
#include "FlexboxLayout.h"

const char *HYComposeFlexboxLayout = "com/temple/skiaui/compose/widget/HYComposeFlexboxLayout";

static JNINativeMethod g_ComposeFlexboxMethods[] = {
};

static int RegisterComposeFlexboxLayoutMethods(JNIEnv *env) {
    ALOGD("RegisterComposeFlexboxLayoutMethods start %s", HYComposeFlexboxLayout)
//    jclass clazz = env->FindClass(HYComposeFlexboxLayout);
//    if (clazz == nullptr) {
//        ALOGD("RegisterComposeFlexboxLayoutMethods fail clazz == null")
//        return JNI_FALSE;
//    }
//    if (env->RegisterNatives(clazz, g_ComposeFlexboxMethods, std::size(g_ComposeFlexboxMethods)) <
//        0) {
//        ALOGD("RegisterComposeFlexboxLayoutMethods fail")
//        return JNI_FALSE;
//    }
    return JNI_TRUE;
}

static void UnRegisterComposeFlexboxLayoutMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeFlexboxLayout);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeFlexboxLayoutMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
