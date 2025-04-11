#pragma once

#include "jni.h"
#include "native_log.h"
#include "ViewGroup.h"
#include "w3c_util.h"

using namespace HYSkiaUI;

const char *HYComposeViewGroup = "com/temple/skiaui/compose/ui/HYComposeViewGroup";

extern "C" JNIEXPORT void JNICALL
compose_view_group_set_direction(JNIEnv *env, jobject instance, jlong viewPtr, jstring value) {
    auto viewGroup = reinterpret_cast<ViewGroup *>(viewPtr);
    auto valueStr = env->GetStringUTFChars(value, nullptr);
    viewGroup->setFlexDirection(W3CToYGFlexDirection(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

extern "C" JNIEXPORT void JNICALL
compose_view_group_set_justify_content(JNIEnv *env, jobject instance, jlong viewPtr,
                                       jstring value) {
    auto viewGroup = reinterpret_cast<ViewGroup *>(viewPtr);
    auto valueStr = env->GetStringUTFChars(value, nullptr);
    viewGroup->setJustifyContent(W3CToYGJustify(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

extern "C" JNIEXPORT void JNICALL
compose_view_group_set_align_items(JNIEnv *env, jobject instance, jlong viewPtr, jstring value) {
    auto viewGroup = reinterpret_cast<ViewGroup *>(viewPtr);
    auto valueStr = env->GetStringUTFChars(value, nullptr);
    viewGroup->setAlignItems(W3CToYGAlign(valueStr));
    env->ReleaseStringUTFChars(value, valueStr);
}

static JNINativeMethod g_ComposeViewGroupMethods[] = {
        {"nativeSetFlexDirection",  "(JLjava/lang/String;)V", (void *) compose_view_group_set_direction},
        {"nativeSetJustifyContent", "(JLjava/lang/String;)V", (void *) compose_view_group_set_justify_content},
        {"nativeSetAlignItems",     "(JLjava/lang/String;)V", (void *) compose_view_group_set_align_items}
};

static int RegisterComposeViewGroupMethods(JNIEnv *env) {
    ALOGD("RegisterComposeViewGroupMethods start %s", HYComposeViewGroup)
    jclass clazz = env->FindClass(HYComposeViewGroup);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeViewGroupMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeViewGroupMethods,
                             std::size(g_ComposeViewGroupMethods)) <
        0) {
        ALOGD("RegisterComposeViewGroupMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeViewGroupMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeViewGroup);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeViewGroupMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
