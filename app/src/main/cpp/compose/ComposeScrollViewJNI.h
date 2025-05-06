#pragma once

#include "jni.h"
#include "ScrollView.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeScrollView = "com/temple/skiaui/compose/ui/HYComposeScrollView";

extern "C" JNIEXPORT void JNICALL
compose_scroll_to_position(JNIEnv *env, jobject instance, jlong viewPtr, jint position) {
    auto scrollView = reinterpret_cast<ScrollView *>(viewPtr);
    if (scrollView != nullptr) {
        scrollView->scrollToIndex(position, true);
    }
}

extern "C" JNIEXPORT void JNICALL
compose_scroll_offset(JNIEnv *env, jobject instance, jlong viewPtr, jint position) {
    auto scrollView = reinterpret_cast<ScrollView *>(viewPtr);
    if (scrollView != nullptr) {
        scrollView->scrollTo(static_cast<float >(position));
    }
}

extern "C" JNIEXPORT jint JNICALL
compose_scroll_get_distance_by_index(JNIEnv *env, jobject instance, jlong viewPtr, jint index) {
    auto scrollView = reinterpret_cast<ScrollView *>(viewPtr);
    if (scrollView != nullptr) {
        return scrollView->getDistanceByIndex(index);
    }
    return 0;
}

static JNINativeMethod g_ComposeScrollViewMethods[] = {
        {"nativeScrollToPosition",   "(JI)V", (void *) compose_scroll_to_position},
        {"nativeScrollOffset",       "(JI)V", (void *) compose_scroll_offset},
        {"nativeGetDistanceByIndex", "(JI)I", (void *) compose_scroll_get_distance_by_index},
};

static int RegisterComposeScrollMethods(JNIEnv *env) {
    ALOGD("RegisterComposeScrollMethods start %s", HYComposeScrollView)
    jclass clazz = env->FindClass(HYComposeScrollView);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeScrollMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeScrollViewMethods,
                             std::size(g_ComposeScrollViewMethods)) < 0) {
        ALOGD("RegisterComposeScrollMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeScrollMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeScrollView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeScrollMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
