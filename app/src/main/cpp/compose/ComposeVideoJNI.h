#include "jni.h"
#include "VideoView.h"
#include "native_log.h"

const char *HYComposeVideo = "com/temple/skiaui/compose/widget/HYComposeVideo";

extern "C" JNIEXPORT jlong JNICALL
compose_video_create_view(JNIEnv *env, jobject instance) {
    auto view = new VideoView();
    return reinterpret_cast<long>(view);
}

extern "C" JNIEXPORT void JNICALL
compose_video_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto videoView = reinterpret_cast<VideoView *>(viewPtr);
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    videoView->setSource(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

static JNINativeMethod g_ComposeVideoViewMethods[] = {
        {"nativeCreateView", "()J",                    (void *) compose_video_create_view},
        {"nativeSetSource",  "(JLjava/lang/String;)V", (void *) compose_video_set_source},
};

static int RegisterComposeVideoMethods(JNIEnv *env) {
    ALOGD("RegisterComposeVideoMethods start %s", HYComposeVideo)
    jclass clazz = env->FindClass(HYComposeVideo);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeVideoMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeVideoViewMethods,
                             std::size(g_ComposeVideoViewMethods)) < 0) {
        ALOGD("RegisterComposeVideoMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}