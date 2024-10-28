#pragma once

#include "jni.h"
#include "native_log.h"
#include "FlexboxLayout.h"
#include "w3c_util.h"

const char *HYComposeFlexboxLayout = "com/temple/skiaui/compose/widget/HYComposeFlexboxLayout";

extern "C" JNIEXPORT void JNICALL
compose_fb_layout_set_direction(JNIEnv *env, jobject instance, jlong viewPtr, jstring value) {
    auto flexboxLayout = reinterpret_cast<FlexboxLayout *>(viewPtr);
    auto valueStr = env->GetStringUTFChars(value, nullptr);
    flexboxLayout->setFlexDirection(W3CToYGFlexDirection(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

extern "C" JNIEXPORT void JNICALL
compose_fb_layout_set_justify_content(JNIEnv *env, jobject instance, jlong viewPtr, jstring value) {
    auto flexboxLayout = reinterpret_cast<FlexboxLayout *>(viewPtr);
    auto valueStr = env->GetStringUTFChars(value, nullptr);
    flexboxLayout->setJustifyContent(W3CToYGJustify(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

extern "C" JNIEXPORT void JNICALL
compose_fb_layout_set_align_items(JNIEnv *env, jobject instance, jlong viewPtr, jstring value) {
    auto flexboxLayout = reinterpret_cast<FlexboxLayout *>(viewPtr);
    auto valueStr = env->GetStringUTFChars(value, nullptr);
    flexboxLayout->setAlignItems(W3CToYGAlign(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

static JNINativeMethod g_ComposeFlexboxMethods[] = {
        {"nativeSetFlexDirection",  "(JLjava/lang/String;)V", (void *) compose_fb_layout_set_direction},
        {"nativeSetJustifyContent", "(JLjava/lang/String;)V", (void *) compose_fb_layout_set_justify_content},
        {"nativeSetAlignItems",     "(JLjava/lang/String;)V", (void *) compose_fb_layout_set_align_items}
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

static void UnRegisterComposeFlexboxLayoutMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeFlexboxLayout);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeFlexboxLayoutMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
