#pragma once

#include "jni.h"
#include "Page.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposePage = "com/temple/skiaui/compose/ui/HYComposePage";

extern "C" JNIEXPORT void JNICALL
compose_page_push(JNIEnv *env, jobject instance, jlong viewPtr, jint width) {
    auto page = reinterpret_cast<Page *>(viewPtr);
    if (page == nullptr) {
        return;
    }
    page->getContext()->getPageStackManager()->push(page);
    page->enterFromRight(Page::EnterExitInfo(width, 0));
}

extern "C" JNIEXPORT void JNICALL
compose_page_pop(JNIEnv *env, jobject instance, jlong viewPtr) {
    auto page = reinterpret_cast<Page *>(viewPtr);
    if (page == nullptr) {
        return;
    }
}

static JNINativeMethod g_ComposePageMethods[] = {
        "nativePush", "(JI)V", (void *) compose_page_push,
        "nativePop", "(J)V", (void *) compose_page_pop,
};

static int RegisterComposePageMethods(JNIEnv *env) {
    ALOGD("RegisterComposePageMethods start %s", HYComposePage)
    jclass clazz = env->FindClass(HYComposePage);
    if (clazz == nullptr) {
        ALOGD("RegisterComposePageMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposePageMethods,
                             std::size(g_ComposePageMethods)) < 0) {
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
