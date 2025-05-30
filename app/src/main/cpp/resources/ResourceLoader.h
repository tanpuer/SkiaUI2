#pragma once

#include <utility>

#include "functional"
#include "any"
#include "native_log.h"
#include "android/SkAnimatedImage.h"
#include "jni.h"
#include "skottie/include/Skottie.h"
#include "svg/include/SkSVGDOM.h"
#include "shared_mutex"

namespace HYSkiaUI {

/**
 * TODO: use concurrent_hash_map instead of shared_mutex + std::unordered_map
 */
class ResourceLoader {

public:

    ResourceLoader(JNIEnv *env, jobject skiaEngine);

    template<typename T>
    void registerCallback(int taskId, std::function<void(T)> &&cb) {
        _callbacks[taskId] = [cb](std::any param) {
            if (auto *ptr = std::any_cast<T>(&param)) {
                cb(*ptr);
            } else {
                ALOGE("illegal param for ResourceLoader")
            }
        };
    }

    void decodeLottie(const std::string &path,
                      std::function<void(sk_sp<skottie::Animation>)> &&callback);

    void
    decodeImage(const std::string &path, std::function<void(sk_sp<SkAnimatedImage>)> &&callback);

    void decodeSVG(const std::string &path, std::function<void(sk_sp<SkSVGDOM>)> &&callback);

    void readFile(const std::string &path, std::function<void(const char *)> &&callback);

    void executeTask(JNIEnv *env, int taskId, jobject javaAssets);

    void postTask(JNIEnv *env, int taskId);

private:

    int generateTaskId();

    void executeTaskInBackground(int taskId);

    void finishTaskInUIThread(JNIEnv* env, int taskId);

    std::unordered_map<int, std::function<void(std::any)>> _callbacks;

    std::unordered_map<int, std::function<std::any(JNIEnv *env, jobject javaAssets)>> _tasks;

    std::unordered_map<int, std::function<void()>> _finishes;

    JNIEnv *env = nullptr;

    jobject skiaEngine = nullptr;

    jmethodID executeTaskMethod = nullptr;

    jmethodID postTaskMethod = nullptr;

    int32_t taskID = 0;

    std::shared_mutex rw_mtx;
};

}
