#pragma once

#include "jni.h"
#include "native_log.h"

using namespace HYSkiaUI;

const char *HYComposePaint = "com/temple/skiaui/compose/ui/HYComposePaint";

extern "C" JNIEXPORT jlong JNICALL
compose_paint_init(JNIEnv *env, jobject instance) {
    auto paint = new SkPaint();
    return reinterpret_cast<long>(paint);
}

extern "C" JNIEXPORT void JNICALL
compose_paint_set_anti_alias(JNIEnv *env, jobject instance, jlong paintPtr, jboolean aa) {
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    paint->setAntiAlias(aa);
}

extern "C" JNIEXPORT void JNICALL
compose_paint_set_color(JNIEnv *env, jobject instance, jlong paintPtr, jint color) {
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    paint->setColor(color);
}

static JNINativeMethod g_ComposePaintMethods[] = {
        {"nativeInitPaint",    "()J",   (void *) compose_paint_init},
        {"nativeSetAntiAlias", "(JZ)V", (void *) compose_paint_set_anti_alias},
        {"nativeSetColor",     "(JI)V", (void *) compose_paint_set_color},
};

static int RegisterComposePaintMethods(JNIEnv *env) {
    ALOGD("RegisterComposePaintMethods start %s", HYComposePaint)
    jclass clazz = env->FindClass(HYComposePaint);
    if (clazz == nullptr) {
        ALOGD("RegisterComposePaintMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposePaintMethods, std::size(g_ComposePaintMethods)) < 0) {
        ALOGD("RegisterComposePaintMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposePaintMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposePaint);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposePaintMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}