#pragma once

#include "jni.h"
#include "YUVVideoView.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeVideo = "com/temple/skiaui/compose/ui/HYComposeVideo";

extern "C" JNIEXPORT void JNICALL
compose_video_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto videoView = reinterpret_cast<YUVVideoView *>(viewPtr);
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    videoView->setSource(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

static JNINativeMethod g_ComposeYUVVideoMethods[] = {
        {"nativeSetSource",  "(JLjava/lang/String;)V", (void *) compose_video_set_source},
};

static int RegisterComposeVideoMethods(JNIEnv *env) {
    ALOGD("RegisterComposeVideoMethods start %s", HYComposeVideo)
    jclass clazz = env->FindClass(HYComposeVideo);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeVideoMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeYUVVideoMethods,
                             std::size(g_ComposeYUVVideoMethods)) < 0) {
        ALOGD("RegisterComposeVideoMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeVideoMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeVideo);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeVideoMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}