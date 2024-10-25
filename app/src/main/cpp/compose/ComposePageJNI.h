#pragma once

#include "jni.h"
#include "Page.h"
#include "native_log.h"

const char *HYComposePage = "com/temple/skiaui/compose/widget/HYComposePage";

extern "C" JNIEXPORT jlong JNICALL
compose_page_create_view(JNIEnv *env, jobject instance, jlong context) {
    auto view = new Page();
    auto ctx = reinterpret_cast<SkiaUIContext *>(context);
    view->setContext(std::shared_ptr<SkiaUIContext>(ctx));
    return reinterpret_cast<long>(view);
}

static JNINativeMethod g_ComposePageMethods[] = {
        {"nativeCreateView", "(J)J", (void *) compose_page_create_view},
};

static int RegisterComposePageMethods(JNIEnv *env) {
    ALOGD("RegisterComposePageMethods start %s", HYComposePage)
    jclass clazz = env->FindClass(HYComposePage);
    if (clazz == nullptr) {
        ALOGD("RegisterComposePageMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposePageMethods, std::size(g_ComposePageMethods)) < 0) {
        ALOGD("RegisterComposePageMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposePageMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposePage);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposePageMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
