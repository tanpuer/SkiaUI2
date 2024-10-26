#pragma once

#include "jni.h"
#include "LoadingView.h"
#include "native_log.h"

const char *HYComposeLoadingView = "com/temple/skiaui/compose/widget/HYComposeLoadingView";

extern "C" JNIEXPORT jlong JNICALL
compose_loading_view_create_view(JNIEnv *env, jobject instance, jlong context) {
    auto view = new LoadingView();
    auto ctx = reinterpret_cast<SkiaUIContext *>(context);
    view->setContext(std::shared_ptr<SkiaUIContext>(ctx));
    return reinterpret_cast<long>(view);
}

static JNINativeMethod g_ComposeLoadingViewMethods[] = {
        {"nativeCreateView", "(J)J", (void *) compose_loading_view_create_view},
};

static int RegisterComposeLoadingViewMethods(JNIEnv *env) {
    ALOGD("RegisterComposeLoadingViewMethods start %s", HYComposeLoadingView)
    jclass clazz = env->FindClass(HYComposeLoadingView);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeLoadingViewMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeLoadingViewMethods,
                             std::size(g_ComposeLoadingViewMethods)) < 0) {
        ALOGD("RegisterComposeLoadingViewMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeLoadingViewMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeLoadingView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeLoadingViewMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
