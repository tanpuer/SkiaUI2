#pragma once

#include "jni.h"
#include "View.h"
#include "native_log.h"

const char *HYComposeView = "com/temple/skiaui/compose/widget/HYComposeView";

extern "C" JNIEXPORT jlong JNICALL
compose_view_create_view(JNIEnv *env, jobject instance) {
    auto view = new View();
    return reinterpret_cast<long>(view);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_width(JNIEnv *env, jobject instance, jlong viewPtr, jint width) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setWidth(width);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_height(JNIEnv *env, jobject instance, jlong viewPtr, jint height) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setHeight(height);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_background_color(JNIEnv *env, jobject instance, jlong viewPtr, jstring color) {
    auto view = reinterpret_cast<View *>(viewPtr);
    auto colorStr = env->GetStringUTFChars(color, nullptr);
    view->setBackgroundColor(colorStr);
    env->ReleaseStringUTFChars(color, colorStr);
}

static JNINativeMethod g_ComposeViewMethods[] = {
        {"nativeCreateView",         "()J",                    (void *) compose_view_create_view},
        {"nativeSetWidth",           "(JI)V",                  (void *) compose_view_set_width},
        {"nativeSetHeight",          "(JI)V",                  (void *) compose_view_set_height},
        {"nativeSetBackgroundColor", "(JLjava/lang/String;)V", (void *) compose_view_set_background_color},
};

static int RegisterComposeViewMethods(JNIEnv *env) {
    ALOGD("RegisterComposeViewMethods start %s", HYComposeView)
    jclass clazz = env->FindClass(HYComposeView);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeViewMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeViewMethods, std::size(g_ComposeViewMethods)) < 0) {
        ALOGD("RegisterComposeViewMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
