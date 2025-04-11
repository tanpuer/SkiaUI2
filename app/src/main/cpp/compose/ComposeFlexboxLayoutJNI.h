#pragma once

#include "jni.h"
#include "native_log.h"
#include "FlexboxLayout.h"
#include "w3c_util.h"

using namespace HYSkiaUI;

const char *HYComposeFlexboxLayout = "com/temple/skiaui/compose/ui/HYComposeFlexboxLayout";

static JNINativeMethod g_ComposeFlexboxMethods[] = {
};

static int RegisterComposeFlexboxLayoutMethods(JNIEnv *env) {
    ALOGD("RegisterComposeFlexboxLayoutMethods start %s", HYComposeFlexboxLayout)
    return JNI_TRUE;
}

static void UnRegisterComposeFlexboxLayoutMethods(JNIEnv *env) {
    ALOGD("UnRegisterComposeFlexboxLayoutMethods")
}
