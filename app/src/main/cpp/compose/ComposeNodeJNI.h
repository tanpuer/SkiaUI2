#pragma once

#include "jni.h"
#include "native_log.h"
#include "ViewGroup.h"
#include "LoadingView.h"
#include "LottieView.h"
#include "ShaderView.h"
#include "ScrollView.h"
#include "Icon.h"
#include "Switch.h"
#include "YUVVideoView.h"
#include "CameraView.h"
#include "FilamentView.h"
#include "ExoPlayerView.h"
#include "WebView.h"
#include "SVGView.h"

using namespace HYSkiaUI;

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
            {"Video",         []() -> View * { return new YUVVideoView(); }},
            {"View",          []() -> View * { return new View(); }},
            {"Loading",       []() -> View * { return new LoadingView(); }},
            {"Lottie",        []() -> View * { return new LottieView(); }},
            {"Shader",        []() -> View * { return new ShaderView(); }},
            {"Text",          []() -> View * { return new TextView(); }},
            {"Scroll",        []() -> View * { return new ScrollView(); }},
            {"Image",         []() -> View * { return new ImageView(); }},
            {"Icon",          []() -> View * { return new Icon(); }},
            {"Switch",        []() -> View * { return new Switch(); }},
            {"Camera",        []() -> View * { return new CameraView(); }},
            {"Filament",      []() -> View * { return new FilamentView(); }},
            {"ExoVideo",      []() -> View * { return new ExoPlayerView(); }},
            {"Web",           []() -> View * { return new WebView(); }},
            {"SVG",           []() -> View * { return new SVGView(); }},
    };
    auto result = viewFactory[typeStr]();
    if (result != nullptr) {
        auto testDraw = reinterpret_cast<ITestDraw *>(contextPtr);
        result->setContext(testDraw->getContext());
        env->ReleaseStringUTFChars(type, typeStr);
        return reinterpret_cast<long >(result);
    }
    env->ReleaseStringUTFChars(type, typeStr);
    ALOGE("UnRegistered View Type:%s", typeStr)
    return 0L;
}

extern "C" JNIEXPORT void JNICALL
compose_node_remove_views(JNIEnv *env, jobject instance, jlong parent, jint index, jint count) {
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    if (viewGroup == nullptr) {
        return;
    }
    for (int i = 0; i < count; ++i) {
        viewGroup->removeViewAt(index);
    }
}

extern "C" JNIEXPORT void JNICALL
compose_node_remove_all_children(JNIEnv *env, jobject instance, jlong parent) {
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    if (viewGroup == nullptr) {
        return;
    }
    viewGroup->removeAllViews();
}

static JNINativeMethod g_ComposeNodeMethods[] = {
        {"nativeAddView",           "(JJ)V",                  (void *) compose_node_add_view},
        {"nativeCreateView",        "(JLjava/lang/String;)J", (void *) compose_node_create_view_factory},
        {"nativeRemoveViews",       "(JII)V",                 (void *) compose_node_remove_views},
        {"nativeRemoveAllChildren", "()V",                    (void *) compose_node_remove_all_children},
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
