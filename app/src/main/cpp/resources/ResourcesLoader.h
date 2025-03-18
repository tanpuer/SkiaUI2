#pragma once

#include "core/SkImage.h"
#include <modules/skottie/include/Skottie.h>
#include "svg/include/SkSVGDOM.h"
#include "jni.h"
#include "unordered_map"
#include "android/SkAnimatedImage.h"
#include "memory"

namespace HYSkiaUI {

static int32_t TASK_ID = 0;
//Todo, post decoding-tasks to thread-pool, then post results to UI-thread.
static std::unordered_map<int, std::function<void(sk_sp<SkAnimatedImage>)>> imagesCallback;
static std::unordered_map<int, std::string> pathMap;
static std::unordered_map<int, sk_sp<SkAnimatedImage>> animatedImagesMap;

static std::unordered_map<int, std::function<void(sk_sp<skottie::Animation>)>> lottieCallback;
static std::unordered_map<int, sk_sp<skottie::Animation>> lottieMap;

static std::unordered_map<int, std::function<void(sk_sp<SkSVGDOM>)>> svgCallback;
static std::unordered_map<int, sk_sp<SkSVGDOM>> svgMap;

static std::unordered_map<int, std::function<void(const char *)>> fileCallback;
static std::unordered_map<int, const char *> fileMap;

class ResourcesLoader {

public:

    ResourcesLoader(JNIEnv *env, jobject skiaEngine) : jniEnv(env), javaSkiaEngine(skiaEngine) {
        javaSkiaEngineClass = env->FindClass("com/temple/skiaui/HYSkiaEngine");
        executeTaskMethod = env->GetMethodID(javaSkiaEngineClass, "executeTask", "(I)V");
        postTaskMethod = env->GetMethodID(javaSkiaEngineClass, "postTask", "(I)V");
    }

    ~ResourcesLoader() = default;

    void decodeImage(const std::string &path,
                     std::function<void(sk_sp<SkAnimatedImage>)> &&callback);

    void decodeLottie(const std::string &path,
                      std::function<void(sk_sp<skottie::Animation>)> &&callback);

    void decodeSVG(const std::string &path, std::function<void(sk_sp<SkSVGDOM>)> &&callback);

    void readFile(const std::string &path, std::function<void(const char *)> &&callback);

    void executeTask(JNIEnv *env, int taskId, jobject javaAssets);

    void postTask(JNIEnv *env, int taskId);

private:
    JNIEnv *jniEnv;
    jclass javaSkiaEngineClass;
    jmethodID executeTaskMethod;
    jmethodID postTaskMethod;
    jobject javaSkiaEngine;
};

}