#pragma once

#include "jni.h"
#include "View.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeView = "com/temple/skiaui/compose/widget/HYComposeView";

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

extern "C" JNIEXPORT void JNICALL
compose_view_set_click_callback(JNIEnv *env, jobject instance, jlong viewPtr) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->checkJavaViewRef(instance);
    view->setOnClickListener([](View *view) -> void {
        auto ctx = view->getContext();
        auto env = ctx->getJniEnv();
        auto javaGlobalRef = view->getJavaViewRef();
        auto triggerClickEventMethodId = env->GetMethodID(env->GetObjectClass(javaGlobalRef),
                                                          "triggerClickEvent",
                                                          "()V");
        env->CallVoidMethod(javaGlobalRef, triggerClickEventMethodId);
    });
}

static JNINativeMethod g_ComposeViewMethods[] = {
        {"nativeSetWidth",           "(JI)V",                  (void *) compose_view_set_width},
        {"nativeSetHeight",          "(JI)V",                  (void *) compose_view_set_height},
        {"nativeSetBackgroundColor", "(JLjava/lang/String;)V", (void *) compose_view_set_background_color},
        {"nativeSetClickCallback",   "(J)V",                   (void *) compose_view_set_click_callback},
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

static void UnRegisterComposeViewMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeViewMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
