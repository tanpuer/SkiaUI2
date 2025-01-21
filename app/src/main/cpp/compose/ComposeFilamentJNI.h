#pragma once

#include "jni.h"
#include "native_log.h"
#include "FilamentView.h"

using namespace HYSkiaUI;

const char *HYComposeFilament = "com/temple/skiaui/compose/widget/HYComposeFilament";

static JNINativeMethod g_ComposeFilamentViewMethods[] = {
};

static int RegisterComposeFilamentMethods(JNIEnv *env) {
    ALOGD("RegisterComposeCameraMethods start %s", HYComposeFilament)
    jclass clazz = env->FindClass(HYComposeFilament);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeCameraMethods fail clazz == null")
        return JNI_FALSE;
    }
//    if (env->RegisterNatives(clazz, g_ComposeFilamentViewMethods,
//                             std::size(g_ComposeFilamentViewMethods)) < 0) {
//        ALOGD("RegisterComposeCameraMethods fail")
//        return JNI_FALSE;
//    }
    return JNI_TRUE;
}

static void UnRegisterComposeFilamentMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeFilament);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeCameraMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
