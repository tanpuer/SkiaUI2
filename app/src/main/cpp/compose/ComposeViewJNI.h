#pragma once

#include "jni.h"
#include "View.h"
#include "native_log.h"
#include "w3c_util.h"

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
compose_view_set_background_color(JNIEnv *env, jobject instance, jlong viewPtr, jint color) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setBackgroundColor(color);
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
    jint *marginsArray = env->GetIntArrayElements(margins, nullptr);
    std::vector<int> marginsVec;
    marginsVec.reserve(length);
    marginsVec.assign(marginsArray, marginsArray + length);
    env->ReleaseIntArrayElements(margins, marginsArray, JNI_ABORT);
    view->setMargin(marginsVec);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_paddings(JNIEnv *env, jobject instance, jlong viewPtr, jintArray margins) {
    auto view = reinterpret_cast<View *>(viewPtr);
    jsize length = env->GetArrayLength(margins);
    jint *marginsArray = env->GetIntArrayElements(margins, nullptr);
    std::vector<int> marginsVec;
    marginsVec.reserve(length);
    marginsVec.assign(marginsArray, marginsArray + length);
    env->ReleaseIntArrayElements(margins, marginsArray, JNI_ABORT);
    view->setPadding(marginsVec);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_position(JNIEnv *env, jobject instance, jlong viewPtr, jstring position) {
    auto view = reinterpret_cast<View *>(viewPtr);
    auto positionStr = env->GetStringUTFChars(position, nullptr);
    view->setPositionType(W3CToYGPosition(positionStr));
    env->ReleaseStringUTFChars(position, positionStr);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_corner_radius(JNIEnv *env, jobject instance, jlong viewPtr, jint radius) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setCornerRadius(radius);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_min_size(JNIEnv *env, jobject instance, jlong viewPtr, jint minWidth,
                          jint minHeight) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setMinSize(minWidth, minHeight);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_align_self(JNIEnv *env, jobject instance, jlong viewPtr, jstring alignSelf) {
    auto view = reinterpret_cast<View *>(viewPtr);
    auto alignSelfStr = env->GetStringUTFChars(alignSelf, nullptr);
    view->setAlignSelf(W3CToYGAlign(alignSelfStr));
    env->ReleaseStringUTFChars(alignSelf, alignSelfStr);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_width_percent(JNIEnv *env, jobject instance, jlong viewPtr, jfloat percent) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setWidthPercent(percent);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_height_percent(JNIEnv *env, jobject instance, jlong viewPtr, jfloat percent) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setHeightPercent(percent);
}

extern "C" JNIEXPORT void JNICALL
compose_view_set_flex(JNIEnv *env, jobject instance, jlong viewPtr, jint flex) {
    auto view = reinterpret_cast<View *>(viewPtr);
    view->setFlex(static_cast<float >(flex));
}

static JNINativeMethod g_ComposeViewMethods[] = {
        {"nativeSetWidth",           "(JI)V",                  (void *) compose_view_set_width},
        {"nativeSetHeight",          "(JI)V",                  (void *) compose_view_set_height},
        {"nativeSetBackgroundColor", "(JI)V",                  (void *) compose_view_set_background_color},
        {"nativeSetClickCallback",   "(J)V",                   (void *) compose_view_set_click_callback},
        {"nativeSetRotateZ",         "(JF)V",                  (void *) compose_view_set_rotateZ},
        {"nativeSetMargins",         "(J[I)V",                 (void *) compose_view_set_margins},
        {"nativeSetPaddings",        "(J[I)V",                 (void *) compose_view_set_paddings},
        {"nativeSetPosition",        "(JLjava/lang/String;)V", (void *) compose_view_set_position},
        {"nativeSetCornerRadius",    "(JI)V",                  (void *) compose_view_set_corner_radius},
        {"nativeSetMinSize",         "(JII)V",                 (void *) compose_view_set_min_size},
        {"nativeSetAlignSelf",       "(JLjava/lang/String;)V", (void *) compose_view_set_align_self},
        {"nativeSetWidthPercent",    "(JF)V",                  (void *) compose_view_set_width_percent},
        {"nativeSetHeightPercent",   "(JF)V",                  (void *) compose_view_set_height_percent},
        {"nativeSetFlex",            "(JI)V",                  (void *) compose_view_set_flex},
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
