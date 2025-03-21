#pragma once

#include "jni.h"
#include "native_log.h"
#include "SVGView.h"

using namespace HYSkiaUI;

const char *HYComposeSVG = "com/temple/skiaui/compose/widget/HYComposeSVG";

extern "C" JNIEXPORT void JNICALL
compose_svg_set_source(JNIEnv *env, jobject instance, jlong viewPtr, jstring source) {
    auto svg = reinterpret_cast<SVGView *>(viewPtr);
    if (svg == nullptr) {
        return;
    }
    auto sourceStr = env->GetStringUTFChars(source, nullptr);
    svg->setSource(sourceStr);
    env->ReleaseStringUTFChars(source, sourceStr);
}

static JNINativeMethod g_ComposeSVGMethods[] = {
        {"nativeSetSource", "(JLjava/lang/String;)V", (void *) compose_svg_set_source},
};

static int RegisterComposeSVGMethods(JNIEnv *env) {
    ALOGD("RegisterComposeSVGMethods start %s", HYComposeSVG)
    jclass clazz = env->FindClass(HYComposeSVG);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeSVGMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeSVGMethods,
                             std::size(g_ComposeSVGMethods)) < 0) {
        ALOGD("RegisterComposeSVGMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeSVGMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeSVG);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeSVGMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}