#pragma once

#include "jni.h"
#include "ExoPlayerView.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeExoVideo = "com/temple/skiaui/compose/ui/HYComposeExoVideo";

extern "C" JNIEXPORT void JNICALL
compose_exo_video_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto videoView = reinterpret_cast<ExoPlayerView *>(viewPtr);
    if (videoView != nullptr) {
        auto sourceStr = env->GetStringUTFChars(source, nullptr);
        videoView->setSource(sourceStr);
        env->ReleaseStringUTFChars(source, sourceStr);
    }
}

extern "C" JNIEXPORT void JNICALL
compose_exo_video_set_custom_player(JNIEnv *env, jobject instance, jlong viewPtr, jobject player) {
    auto videoView = reinterpret_cast<ExoPlayerView *>(viewPtr);
    if (videoView != nullptr) {
        videoView->setCustomVideoPlayer(player);
    }
}

static JNINativeMethod g_ComposeExoVideoMethods[] = {
        {"nativeSetSource",       "(JLjava/lang/String;)V", (void *) compose_exo_video_set_source},
        {"nativeSetCustomPlayer", "(JLcom/temple/skiaui/platform/video/IVideoPlayer;)V",
                                                            (void *) compose_exo_video_set_custom_player},
};

static int RegisterComposeExoVideoMethods(JNIEnv *env) {
    ALOGD("RegisterComposeVideoMethods start %s", HYComposeExoVideo)
    jclass clazz = env->FindClass(HYComposeExoVideo);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeVideoMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeExoVideoMethods,
                             std::size(g_ComposeExoVideoMethods)) < 0) {
        ALOGD("RegisterComposeVideoMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeExoVideoMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeExoVideo);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeVideoMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}