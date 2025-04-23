#pragma once

#include "jni.h"
#include "native_log.h"
#include "ComposeRecyclerView.h"

using namespace HYSkiaUI;

const char *HYComposeRecyclerView = "com/temple/skiaui/compose/ui/HYComposeRecyclerView";

extern "C" JNIEXPORT void JNICALL
compose_recycler_view_set_size(JNIEnv *env, jobject instance, jlong viewPtr, jint size) {
    auto recyclerView = reinterpret_cast<ComposeRecyclerView *>(viewPtr);
    if (recyclerView != nullptr) {
        recyclerView->setDataSize(static_cast<uint32_t>(size));
    }
}

static JNINativeMethod g_ComposeRecyclerMethods[] = {
        {"nativeSetDataSize", "(JI)V", (void *) compose_recycler_view_set_size},
};

static int RegisterComposeRecyclerMethods(JNIEnv *env) {
    ALOGD("RegisterComposeRecyclerMethods start %s", HYComposeRecyclerView)
    jclass clazz = env->FindClass(HYComposeRecyclerView);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeRecyclerMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeRecyclerMethods, std::size(g_ComposeRecyclerMethods)) <
        0) {
        ALOGD("RegisterComposeRecyclerMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeRecyclerMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeRecyclerView);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeRecyclerMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
