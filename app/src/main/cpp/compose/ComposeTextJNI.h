#include "jni.h"
#include "TextView.h"
#include "native_log.h"

const char *HYComposeText = "com/temple/skiaui/compose/widget/HYComposeText";

extern "C" JNIEXPORT void JNICALL
compose_video_set_text(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    textView->setText(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

extern "C" JNIEXPORT void JNICALL
compose_video_set_text_size(JNIEnv *env, jobject instance, jlong viewPtr, jint size) {
    auto textView = reinterpret_cast<TextView *>(viewPtr);
    textView->setTextSize(size);
}

static JNINativeMethod g_ComposeVideoViewMethods[] = {
        {"nativeSetText", "(JLjava/lang/String;)V", (void *) compose_video_set_text},
        {"nativeSetTextSize", "(JI)V", (void *) compose_video_set_text_size}
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