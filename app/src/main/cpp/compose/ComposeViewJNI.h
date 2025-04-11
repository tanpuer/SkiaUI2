#pragma once

#include "jni.h"
#include "View.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeView = "com/temple/skiaui/compose/ui/HYComposeView";

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

extern "C" JNIEXPORT void JNICALL
compose_view_set_rotateZ(JNIEnv *env, jobject instance, jlong viewPtr, jfloat rotateZ) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setRotateZ(rotateZ);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_margins(JNIEnv *env, jobject instance, jlong viewPtr, jintArray margins) {
    auto view = reinterpret_cast<View *>(viewPtr);
    jsize length = env->GetArrayLength(margins);
    jint* marginsArray = env->GetIntArrayElements(margins, nullptr);
    std::vector<int> marginsVec;
    marginsVec.reserve(length);
    marginsVec.assign(marginsArray, marginsArray + length);
    env->ReleaseIntArrayElements(margins, marginsArray, JNI_ABORT);
    view->setMargin(marginsVec);
}

static JNINativeMethod g_ComposeViewMethods[] = {
        {"nativeSetWidth",           "(JI)V",                  (void *) compose_view_set_width},
        {"nativeSetHeight",          "(JI)V",                  (void *) compose_view_set_height},
        {"nativeSetBackgroundColor", "(JLjava/lang/String;)V", (void *) compose_view_set_background_color},
        {"nativeSetClickCallback",   "(J)V",                   (void *) compose_view_set_click_callback},
        {"nativeSetRotateZ",         "(JF)V",                  (void *) compose_view_set_rotateZ},
        {"nativeSetMargins",         "(J[I)V",                 (void *) compose_view_set_margins},
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
