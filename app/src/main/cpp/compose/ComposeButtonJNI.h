#pragma once

#include "jni.h"
#include "Button.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeButton = "com/temple/skiaui/compose/ui/HYComposeButton";

static JNINativeMethod g_ComposeButtonMethods[] = {
};

static int RegisterComposeButtonMethods(JNIEnv *env) {
    return JNI_TRUE;
}

static void UnRegisterComposeButtonMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeButton);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeButtonMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}