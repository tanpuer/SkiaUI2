#pragma once

#include "jni.h"
#include "native_log.h"
#include "android/bitmap.h"
#include "core/SkPath.h"

using namespace HYSkiaUI;

const char *HYComposePath = "com/temple/skiaui/compose/ui/HYComposePath";

extern "C" JNIEXPORT jlong JNICALL
compose_path_init(JNIEnv *env, jobject instance) {
    auto path = new SkPath();
    return reinterpret_cast<long>(path);
}

extern "C" JNIEXPORT void JNICALL
compose_path_move_to(JNIEnv *env, jobject instance, jlong ref, float x, float y) {
    auto path = reinterpret_cast<SkPath *>(ref);
    path->moveTo(x, y);
}

extern "C" JNIEXPORT void JNICALL
compose_path_line_to(JNIEnv *env, jobject instance, jlong ref, float x, float y) {
    auto path = reinterpret_cast<SkPath *>(ref);
    path->lineTo(x, y);
}

extern "C" JNIEXPORT void JNICALL
compose_path_close(JNIEnv *env, jobject instance, jlong ref, float x, float y) {
    auto path = reinterpret_cast<SkPath *>(ref);
    path->close();
}

extern "C" JNIEXPORT void JNICALL
compose_path_de_init(JNIEnv *env, jobject instance, jlong ref) {
    auto path = reinterpret_cast<SkPath *>(ref);
    delete path;
}

static JNINativeMethod g_ComposePathMethods[] = {
        {"nativeInit",   "()J",    (void *) compose_path_init},
        {"nativeMoveTo", "(JFF)V", (void *) compose_path_move_to},
        {"nativeLineTo", "(JFF)V", (void *) compose_path_line_to},
        {"nativeClose",  "(J)V",   (void *) compose_path_close},
        {"nativeDeInit", "(J)V",   (void *) compose_path_de_init},
};

static int RegisterComposePathMethods(JNIEnv *env) {
    ALOGD("RegisterComposePathMethods start %s", HYComposePath)
    jclass clazz = env->FindClass(HYComposePath);
    if (clazz == nullptr) {
        ALOGD("RegisterComposePathMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposePathMethods, std::size(g_ComposePathMethods)) <
        0) {
        ALOGD("RegisterComposePathMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposePathMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposePath);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposePathMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}