#pragma once

#include "jni.h"
#include "native_log.h"
#include "ImageView.h"

using namespace HYSkiaUI;

const char *HYComposeImage = "com/temple/skiaui/compose/ui/HYComposeImage";

extern "C" JNIEXPORT void JNICALL
compose_image_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto imageView = reinterpret_cast<ImageView *>(viewPtr);
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    imageView->setSource(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

static JNINativeMethod g_ComposeImageViewMethods[] = {
        {"nativeSetSource", "(JLjava/lang/String;)V", (void *) compose_image_set_source},
};

static int RegisterComposeImageMethods(JNIEnv *env) {
    ALOGD("RegisterComposeImageMethods start %s", HYComposeImage)
    jclass clazz = env->FindClass(HYComposeImage);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeImageMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeImageViewMethods,
                             std::size(g_ComposeImageViewMethods)) < 0) {
        ALOGD("RegisterComposeImageMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeImageMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeImage);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeImageMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}