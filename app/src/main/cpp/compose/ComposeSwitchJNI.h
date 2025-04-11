#pragma once

#include "jni.h"
#include "Switch.h"
#include "native_log.h"
#include "color_util.h"

using namespace HYSkiaUI;

const char *HYComposeSwitch = "com/temple/skiaui/compose/ui/HYComposeSwitch";

extern "C" JNIEXPORT void JNICALL
compose_switch_set_enable(JNIEnv *env, jobject instance, jlong viewPtr, jboolean enable) {
    auto view = reinterpret_cast<Switch *>(viewPtr);
    if (view == nullptr) {
        return;
    }
    view->setEnabled(enable);
}

extern "C" JNIEXPORT void JNICALL
compose_switch_set_color(JNIEnv *env, jobject instance, jlong viewPtr, jstring color) {
    auto view = reinterpret_cast<Switch *>(viewPtr);
    if (view == nullptr) {
        return;
    }
    auto colorStr = env->GetStringUTFChars(color, nullptr);
    int r, g, b, a;
    hexToRGBA(colorStr, r, g, b, a);
    view->setColor(SkColorSetARGB(a, r, g, b));
    env->ReleaseStringUTFChars(color, colorStr);
}

static JNINativeMethod g_ComposeSwitchViewMethods[] = {
        {"nativeSetEnable", "(JZ)V",                  (void *) compose_switch_set_enable},
        {"nativeSetColor",  "(JLjava/lang/String;)V", (void *) compose_switch_set_color},
};

static int RegisterComposeSwitchMethods(JNIEnv *env) {
    ALOGD("RegisterComposeSwitchMethods start %s", HYComposeSwitch)
    jclass clazz = env->FindClass(HYComposeSwitch);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeSwitchMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeSwitchViewMethods,
                             std::size(g_ComposeSwitchViewMethods)) < 0) {
        ALOGD("RegisterComposeSwitchMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeSwitchMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeSwitch);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeSwitchMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}