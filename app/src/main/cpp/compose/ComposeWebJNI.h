#pragma once

#include "jni.h"
#include "WebView.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeWeb = "com/temple/skiaui/compose/widget/HYComposeWeb";

extern "C" JNIEXPORT void JNICALL
compose_web_load_url(JNIEnv *env, jobject instance, jlong viewPtr, jstring url) {
    auto webView = reinterpret_cast<WebView *>(viewPtr);
    auto webUrl = env->GetStringUTFChars(url, nullptr);
    webView->loadUrl(webUrl);
    env->ReleaseStringUTFChars(url, webUrl);
}

static JNINativeMethod g_ComposeWebMethods[] = {
        {"nativeLoadUrl",  "(JLjava/lang/String;)V", (void *) compose_web_load_url},
};

static int RegisterComposeWebMethods(JNIEnv *env) {
    ALOGD("RegisterComposeWebMethods start %s", HYComposeWeb)
    jclass clazz = env->FindClass(HYComposeWeb);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeWebMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeWebMethods,
                             std::size(g_ComposeWebMethods)) < 0) {
        ALOGD("RegisterComposeWebMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeWebMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeWeb);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeWebMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}