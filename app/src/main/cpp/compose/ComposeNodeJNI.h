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
#include "Button.h"
#include "EditText.h"
#include "ComposeContext.h"
#include "ProgressBar.h"
#include "ComposeRecyclerView.h"
#include "AndroidImageView.h"

using namespace HYSkiaUI;

const char *HYComposeNode = "com/temple/skiaui/compose/ui/HYComposeNode";

extern "C" JNIEXPORT void JNICALL
compose_node_add_view(JNIEnv *env, jobject instance, jlong parent, jlong child, jint index) {
    ALOGD("compose_node_add_view")
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    auto view = reinterpret_cast<View *>(child);
    viewGroup->addViewAt(view, index);
}

extern "C" JNIEXPORT jlong JNICALL
compose_node_create_view_factory(JNIEnv *env, jobject instance, jstring type) {
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
            {"Button",        []() -> View * { return new Button(); }},
            {"EditText",      []() -> View * { return new EditText(); }},
            {"ProgressBar",   []() -> View * { return new ProgressBar(); }},
            {"RecyclerView",  []() -> View * { return new ComposeRecyclerView(); }},
            {"AndroidImage",  []() -> View * { return new AndroidImageView(); }},
    };
    auto result = viewFactory[typeStr]();
    auto context = ComposeContext::getInstance()->getUIContext();
    result->setContext(context);
    result->checkJavaViewRef(instance);
    env->ReleaseStringUTFChars(type, typeStr);
    return reinterpret_cast<long >(result);
}

extern "C" JNIEXPORT void JNICALL
compose_node_remove_views(JNIEnv *env, jobject instance, jlong parent, jint index, jint count) {
    ALOGD("compose_node_remove_views %d", count)
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    if (viewGroup == nullptr) {
        return;
    }
    viewGroup->removeViews(index, count);
}

extern "C" JNIEXPORT void JNICALL
compose_node_move_views(JNIEnv *env, jobject instance, jlong parent, jint from, jint to,
                        jint count) {
    ALOGD("compose_node_remove_views %d", count)
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    if (viewGroup == nullptr) {
        return;
    }
    viewGroup->moveViews(from, to, count);
}

extern "C" JNIEXPORT void JNICALL
compose_node_remove_all_children(JNIEnv *env, jobject instance, jlong parent) {
    auto viewGroup = reinterpret_cast<ViewGroup *>(parent);
    if (viewGroup == nullptr) {
        return;
    }
    viewGroup->removeViews(0, viewGroup->children.size());
}

static JNINativeMethod g_ComposeNodeMethods[] = {
        {"nativeAddView",           "(JJI)V",                (void *) compose_node_add_view},
        {"nativeCreateView",        "(Ljava/lang/String;)J", (void *) compose_node_create_view_factory},
        {"nativeRemoveViews",       "(JII)V",                (void *) compose_node_remove_views},
        {"nativeMove",              "(JIII)V",               (void *) compose_node_move_views},
        {"nativeRemoveAllChildren", "()V",                   (void *) compose_node_remove_all_children},
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
