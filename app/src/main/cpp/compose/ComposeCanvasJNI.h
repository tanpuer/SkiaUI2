#pragma once

#include "jni.h"
#include "native_log.h"
#include "ComposeCanvas.h"

using namespace HYSkiaUI;

const char *HYComposeCanvas = "com/temple/skiaui/compose/ui/HYComposeCanvas";

extern "C" JNIEXPORT void JNICALL
compose_canvas_draw_rect(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat l, jfloat t,
                         jfloat r, jfloat b, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawRect(SkRect::MakeLTRB(l, t, r, b), *paint);
}

static JNINativeMethod g_ComposeCanvasMethods[] = {
        {"nativeDrawRect", "(JFFFFJ)V", (void *) compose_canvas_draw_rect},
};

static int RegisterComposeCanvasMethods(JNIEnv *env) {
    ALOGD("RegisterComposeCanvasMethods start %s", HYComposeCanvas)
    jclass clazz = env->FindClass(HYComposeCanvas);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeCanvasMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeCanvasMethods, std::size(g_ComposeCanvasMethods)) <
        0) {
        ALOGD("RegisterComposeCanvasMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeCanvasMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeCanvas);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeCanvasMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}