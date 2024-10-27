#include "jni.h"
#include "native_log.h"
#include "ShaderView.h"

const char *HYComposeShader = "com/temple/skiaui/compose/widget/HYComposeShader";

extern "C" JNIEXPORT void JNICALL
compose_shader_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto shaderView = reinterpret_cast<ShaderView *>(viewPtr);
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    shaderView->setShaderPath(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

static JNINativeMethod g_ComposeShaderViewMethods[] = {
        {"nativeSetSource", "(JLjava/lang/String;)V", (void *) compose_shader_set_source},
};

static int RegisterComposeShaderMethods(JNIEnv *env) {
    ALOGD("RegisterComposeShaderMethods start %s", HYComposeShader)
    jclass clazz = env->FindClass(HYComposeShader);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeShaderMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeShaderViewMethods,
                             std::size(g_ComposeShaderViewMethods)) < 0) {
        ALOGD("RegisterComposeShaderMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeShaderMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeShader);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeShaderMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}