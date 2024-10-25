#pragma once

#include "jni.h"
#include "native_log.h"
#include "ViewGroup.h"

const char *HYComposeNode = "com/temple/skiaui/compose/widget/HYComposeNode";

extern "C" JNIEXPORT void JNICALL
compose_node_add_view(JNIEnv *env, jobject instance, jlong parent, jlong child) {
    ALOGD("native_AddView")
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    auto view = reinterpret_cast<View *>(child);
    viewGroup->addView(view);
}

static JNINativeMethod g_ComposeNodeMethods[] = {
        {"nativeAddView", "(JJ)V", (void *) compose_node_add_view},
};

static int RegisterComposeNodeMethods(JNIEnv *env) {
    ALOGD("RegisterComposeNodeMethods start %s", HYComposeNode)
    jclass clazz = env->FindClass(HYComposeNode);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeNodeMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeNodeMethods, std::size(g_ComposeNodeMethods)) < 0) {
        ALOGD("RegisterComposeNodeMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeNodeMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeNode);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeNodeMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
