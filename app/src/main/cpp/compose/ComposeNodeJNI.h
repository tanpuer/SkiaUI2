#pragma once

#include "jni.h"
#include "native_log.h"
#include "ViewGroup.h"
#include "VideoView.h"
#include "LoadingView.h"

const char *HYComposeNode = "com/temple/skiaui/compose/widget/HYComposeNode";

extern "C" JNIEXPORT void JNICALL
compose_node_add_view(JNIEnv *env, jobject instance, jlong parent, jlong child) {
    ALOGD("native_AddView")
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    auto view = reinterpret_cast<View *>(child);
    viewGroup->addView(view);
}

extern "C" JNIEXPORT jlong JNICALL
compose_node_create_view_factory(JNIEnv *env, jobject instance, jlong contextPtr, jstring type) {
    ALOGD("nativeCreateView")
    auto typeStr = env->GetStringUTFChars(type, nullptr);
    static std::unordered_map<std::string, std::function<View *()>> viewFactory = {
            {"Page",          []() -> View * { return new Page(); }},
            {"FlexboxLayout", []() -> View * { return new FlexboxLayout(); }},
            {"Video",         []() -> View * { return new VideoView(); }},
            {"View",          []() -> View * { return new View(); }},
            {"Loading",       []() -> View * { return new LoadingView(); }},
    };
    auto result = viewFactory[typeStr]();
    if (result != nullptr) {
        auto ctx = reinterpret_cast<SkiaUIContext *>(contextPtr);
        result->setContext(std::shared_ptr<SkiaUIContext>(ctx));
        env->ReleaseStringUTFChars(type, typeStr);
        return reinterpret_cast<long >(result);
    }
    env->ReleaseStringUTFChars(type, typeStr);
    ALOGE("UnRegistered View Type:%s", typeStr)
    return 0L;
}

static JNINativeMethod g_ComposeNodeMethods[] = {
        {"nativeAddView",    "(JJ)V",                  (void *) compose_node_add_view},
        {"nativeCreateView", "(JLjava/lang/String;)J", (void *) compose_node_create_view_factory},
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
