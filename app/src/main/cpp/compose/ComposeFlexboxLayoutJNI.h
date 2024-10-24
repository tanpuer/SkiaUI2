#pragma once

#include "jni.h"
#include "native_log.h"
#include "FlexboxLayout.h"

const char *HYComposeFlexboxLayout = "com/temple/skiaui/compose/widget/HYComposeFlexboxLayout";

extern "C" JNIEXPORT jlong JNICALL
compose_flexbox_create_view(JNIEnv *env, jobject instance) {
    auto flexboxLayout = new FlexboxLayout();
    return reinterpret_cast<long>(flexboxLayout);
}

static JNINativeMethod g_ComposeFlexboxMethods[] = {
        {"nativeCreateView", "()J", (void *) compose_flexbox_create_view},
};

static int RegisterComposeFlexboxLayoutMethods(JNIEnv *env) {
    ALOGD("RegisterComposeFlexboxLayoutMethods start %s", HYComposeFlexboxLayout)
    jclass clazz = env->FindClass(HYComposeFlexboxLayout);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeFlexboxLayoutMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeFlexboxMethods, std::size(g_ComposeFlexboxMethods)) <
        0) {
        ALOGD("RegisterComposeFlexboxLayoutMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
