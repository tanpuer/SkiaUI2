#pragma once

#include "jni.h"
#include "EditText.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeEditText = "com/temple/skiaui/compose/ui/HYComposeEditText";

extern "C" JNIEXPORT void JNICALL
compose_edit_set_hint(JNIEnv *env, jobject instance, jlong viewPtr, jstring hint) {
    auto editText = reinterpret_cast<EditText *>(viewPtr);
    auto hintString = env->GetStringUTFChars(hint, nullptr);
    editText->setHint(hintString);
    env->ReleaseStringUTFChars(hint, hintString);
}

extern "C" JNIEXPORT void JNICALL
compose_edit_set_focus(JNIEnv *env, jobject instance, jlong viewPtr, jboolean focus) {
    auto editText = reinterpret_cast<EditText *>(viewPtr);
    if (focus) {
        editText->requestFocus();
    } else {
        editText->clearFocus();
    }
}

static JNINativeMethod g_ComposeEditTextMethods[] = {
        {"nativeSetHint",  "(JLjava/lang/String;)V", (void *) compose_edit_set_hint},
        {"nativeSetFocus", "(JZ)V",                  (void *) compose_edit_set_focus},
};

static int RegisterComposeEditTextMethods(JNIEnv *env) {
    ALOGD("RegisterComposeEditTextMethods start %s", HYComposeEditText)
    jclass clazz = env->FindClass(HYComposeEditText);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeEditTextMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeEditTextMethods,
                             std::size(g_ComposeEditTextMethods)) < 0) {
        ALOGD("RegisterComposeEditTextMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeEditTextMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeEditText);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeEditTextMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}