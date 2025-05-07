#pragma once

#include "jni.h"
#include "BaseSurfaceTextureView.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposeBaseSurfaceTexture = "com/temple/skiaui/compose/ui/HYComposeBaseSurfaceTexture";

extern "C" JNIEXPORT void JNICALL
compose_base_surface_texture_set_shader_path(JNIEnv *env, jobject instance, jlong viewPtr,
                                             jstring path) {
    auto textureView = reinterpret_cast<BaseSurfaceTextureView *>(viewPtr);
    auto pathStr = env->GetStringUTFChars(path, nullptr);
    textureView->setShaderPath(pathStr);
    env->ReleaseStringUTFChars(path, pathStr);
}

extern "C" JNIEXPORT void JNICALL
compose_base_surface_texture_set_shader_code(JNIEnv *env, jobject instance, jlong viewPtr,
                                             jstring code) {
    auto textureView = reinterpret_cast<BaseSurfaceTextureView *>(viewPtr);
    auto codeStr = env->GetStringUTFChars(code, nullptr);
    textureView->setShaderCode(codeStr);
    env->ReleaseStringUTFChars(code, codeStr);
}

static JNINativeMethod g_ComposeBaseSurfaceTextureMethods[] = {
        {"nativeSetShaderPath", "(JLjava/lang/String;)V",
         (void *) compose_base_surface_texture_set_shader_path},
        {"nativeSetShaderCode", "(JLjava/lang/String;)V",
         (void *) compose_base_surface_texture_set_shader_code},
};

static int RegisterComposeBaseSurfaceTextureMethods(JNIEnv *env) {
    ALOGD("RegisterComposeBaseSurfaceTextureMethods start %s", HYComposeBaseSurfaceTexture)
    jclass clazz = env->FindClass(HYComposeBaseSurfaceTexture);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeBaseSurfaceTextureMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeBaseSurfaceTextureMethods,
                             std::size(g_ComposeBaseSurfaceTextureMethods)) < 0) {
        ALOGD("RegisterComposeBaseSurfaceTextureMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeBaseSurfaceTextureMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeBaseSurfaceTexture);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeBaseSurfaceTextureMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}