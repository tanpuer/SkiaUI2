#pragma once

#include "jni.h"
#include "Icon.h"
#include "native_log.h"
#include "color_util.h"

const char *HYComposeIcon = "com/temple/skiaui/compose/widget/HYComposeIcon";

extern "C" JNIEXPORT void JNICALL
compose_icon_set_icon(JNIEnv *env, jobject instance, jlong viewPtr, jint iconValue) {
    auto icon = reinterpret_cast<Icon *>(viewPtr);
    if (icon == nullptr) {
        return;
    }
    icon->setIcon(iconValue);
}

extern "C" JNIEXPORT void JNICALL
compose_icon_set_size(JNIEnv *env, jobject instance, jlong viewPtr, jint size) {
    auto icon = reinterpret_cast<Icon *>(viewPtr);
    if (icon == nullptr) {
        return;
    }
    icon->setIconSize(size);
}

extern "C" JNIEXPORT void JNICALL
compose_icon_set_color(JNIEnv *env, jobject instance, jlong viewPtr, jstring color) {
    auto icon = reinterpret_cast<Icon *>(viewPtr);
    if (icon == nullptr) {
        return;
    }
    auto colorStr = env->GetStringUTFChars(color, nullptr);
    int r, g, b, a;
    hexToRGBA(colorStr, r, g, b, a);
    icon->setIconColor(SkColorSetARGB(a, r, g, b));
    env->ReleaseStringUTFChars(color, colorStr);
}

static JNINativeMethod g_ComposeIconViewMethods[] = {
        {"nativeSetIcon",  "(JI)V",                  (void *) compose_icon_set_icon},
        {"nativeSetSize",  "(JI)V",                  (void *) compose_icon_set_size},
        {"nativeSetColor", "(JLjava/lang/String;)V", (void *) compose_icon_set_color},
};

static int RegisterComposeIconMethods(JNIEnv *env) {
    ALOGD("RegisterComposeIconMethods start %s", HYComposeIcon)
    jclass clazz = env->FindClass(HYComposeIcon);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeIconMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeIconViewMethods,
                             std::size(g_ComposeIconViewMethods)) < 0) {
        ALOGD("RegisterComposeIconMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeIconMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeIcon);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeIconMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}