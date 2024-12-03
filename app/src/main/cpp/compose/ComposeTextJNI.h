#pragma once

#include "jni.h"
#include "TextView.h"
#include "native_log.h"
#include "color_util.h"

using namespace HYSkiaUI;

const char *HYComposeText = "com/temple/skiaui/compose/widget/HYComposeText";

extern "C" JNIEXPORT void JNICALL
compose_text_set_text(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    if (textView == nullptr) {
        return;
    }
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    textView->setText(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

extern "C" JNIEXPORT void JNICALL
compose_text_set_text_size(JNIEnv *env, jobject instance, jlong viewPtr, jint size) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    if (textView == nullptr) {
        return;
    }
    textView->setTextSize(size);
}

extern "C" JNIEXPORT void JNICALL
compose_text_set_color(JNIEnv *env, jobject instance, jlong viewPtr, jstring color) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    if (textView == nullptr) {
        return;
    }
    auto colorStr = env->GetStringUTFChars(color, nullptr);
    int r, g, b, a;
    hexToRGBA(colorStr, r, g, b, a);
    textView->setTextColor(SkColorSetARGB(a, r, g, b));
    env->ReleaseStringUTFChars(color, colorStr);
}

extern "C" JNIEXPORT void JNICALL
compose_text_set_max_line(JNIEnv *env, jobject instance, jlong viewPtr, jint maxLine) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    if (textView == nullptr) {
        return;
    }
    textView->setMaxLines(maxLine);
}

extern "C" JNIEXPORT void JNICALL
compose_text_set_ellipse(JNIEnv *env, jobject instance, jlong viewPtr, jstring ellipse) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    if (textView == nullptr) {
        return;
    }
    auto ellipseStr = env->GetStringUTFChars(ellipse, nullptr);
    textView->setEllipsis(ellipseStr);
    env->ReleaseStringUTFChars(ellipse, ellipseStr);
}

static JNINativeMethod g_ComposeVideoViewMethods[] = {
        {"nativeSetText",     "(JLjava/lang/String;)V", (void *) compose_text_set_text},
        {"nativeSetTextSize", "(JI)V",                  (void *) compose_text_set_text_size},
        {"nativeSetColor",    "(JLjava/lang/String;)V", (void *) compose_text_set_color},
        {"nativeSetMaxLine",  "(JI)V",                  (void *) compose_text_set_max_line},
        {"nativeSetEllipsis", "(JLjava/lang/String;)V", (void *) compose_text_set_ellipse},
};

static int RegisterComposeTextMethods(JNIEnv *env) {
    ALOGD("RegisterComposeTextMethods start %s", HYComposeText)
    jclass clazz = env->FindClass(HYComposeText);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeTextMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeVideoViewMethods,
                             std::size(g_ComposeVideoViewMethods)) < 0) {
        ALOGD("RegisterComposeTextMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeTextMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeText);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeTextMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}