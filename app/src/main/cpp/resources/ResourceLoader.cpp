#include "ResourceLoader.h"
#include "AssetManager.h"
#include "codec/SkAndroidCodec.h"
#include "core/SkImage.h"
#include "core/SkStream.h"

namespace HYSkiaUI {

ResourceLoader::ResourceLoader(JNIEnv *env, jobject skiaEngine) {
    this->env = env;
    this->skiaEngine = skiaEngine;
    auto javaSkiaEngineClass = env->FindClass("com/temple/skiaui/HYSkiaEngine");
    executeTaskMethod = env->GetMethodID(javaSkiaEngineClass, "executeTask", "(I)V");
    postTaskMethod = env->GetMethodID(javaSkiaEngineClass, "postTask", "(I)V");
}

void ResourceLoader::executeTask(JNIEnv *env, int taskId, jobject javaAssets) {
    std::function<std::any(JNIEnv *env, jobject javaAssets)> func = nullptr;
    {
        std::shared_lock<std::shared_mutex> lock(rw_mtx);
        if (_tasks.find(taskId) == _tasks.end()) {
            return;
        }
        func = _tasks[taskId];
        _tasks.erase(taskId);
    }
    if (func == nullptr) {
        return;
    }
    auto result = func(env, javaAssets);
    {
        std::shared_lock<std::shared_mutex> lock(rw_mtx);
        _finishes[taskId] = [result, this, taskId]() {
            if (_callbacks.find(taskId) != _callbacks.end()) {
                _callbacks[taskId](result);
                _callbacks.erase(taskId);
            }
        };
    }
    finishTaskInUIThread(env, taskId);
}

void ResourceLoader::postTask(JNIEnv *env, int taskId) {
    std::function<void()> func = nullptr;
    {
        std::shared_lock<std::shared_mutex> lock(rw_mtx);
        if (_finishes.find(taskId) == _finishes.end()) {
            return;
        }
        func = _finishes[taskId];
        _finishes.erase(taskId);
    }
    if (func != nullptr) {
        func();
    }
}

void ResourceLoader::decodeLottie(const std::string &path, std::function<void(
        sk_sp<skottie::Animation>)> &&callback) {
    auto taskId = generateTaskId();
    {
        std::unique_lock<std::shared_mutex> lock(rw_mtx);
        registerCallback(taskId, std::move(callback));
        _tasks[taskId] = [path](JNIEnv *env, jobject javaAssets) -> sk_sp<skottie::Animation> {
            auto assetManager = AssetManager(env, javaAssets);
            auto imageData = assetManager.readImage(path.c_str());
            auto length = imageData->length;
            auto lottieAnimation = skottie::Animation::Make(
                    reinterpret_cast<const char *>(imageData->content), length);
            return lottieAnimation;
        };
    }
    executeTaskInBackground(taskId);
}

void ResourceLoader::decodeSVG(const std::string &path,
                               std::function<void(sk_sp<SkSVGDOM>)> &&callback) {
    auto taskId = generateTaskId();
    {
        std::unique_lock<std::shared_mutex> lock(rw_mtx);
        registerCallback(taskId, std::move(callback));
        _tasks[taskId] = [path](JNIEnv *env, jobject javaAssets) -> sk_sp<SkSVGDOM> {
            auto assetManager = AssetManager(env, javaAssets);
            auto imageData = assetManager.readImage(path.c_str());
            auto length = imageData->length;
            auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
            auto stream = SkMemoryStream::Make(skData);
            auto skSVGDom = SkSVGDOM::Builder().make(*stream);
            return skSVGDom;
        };
    }
    executeTaskInBackground(taskId);
}

void ResourceLoader::readFile(const std::string &path,
                              std::function<void(const char *)> &&callback) {
    auto taskId = generateTaskId();
    {
        std::unique_lock<std::shared_mutex> lock(rw_mtx);
        registerCallback(taskId, std::move(callback));
        _tasks[taskId] = [path](JNIEnv *env, jobject javaAssets) -> const char * {
            auto assetManager = AssetManager(env, javaAssets);
            return assetManager.readFile(path.c_str());
        };
    }
    executeTaskInBackground(taskId);
}

void ResourceLoader::decodeImage(const std::string &path,
                                 std::function<void(sk_sp<SkAnimatedImage>)> &&callback) {
    auto taskId = generateTaskId();
    {
        std::unique_lock<std::shared_mutex> lock(rw_mtx);
        registerCallback(taskId, std::move(callback));
        _tasks[taskId] = [path](JNIEnv *env, jobject javaAssets) -> sk_sp<SkAnimatedImage> {
            auto assetManager = AssetManager(env, javaAssets);
            auto imageData = assetManager.readImage(path.c_str());
            auto length = imageData->length;
            auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
            auto androidCodec = SkAndroidCodec::MakeFromData(skData);
            auto skAnimatedImage = SkAnimatedImage::Make(std::move(androidCodec));
            ALOGD("animated info : %d %d %d", skAnimatedImage->getFrameCount(),
                  skAnimatedImage->getRepetitionCount(), skAnimatedImage->currentFrameDuration())
            auto frame = skAnimatedImage->getCurrentFrame();
            if (frame == nullptr) {
                ALOGE("skImage is null, pls check %s", path.c_str())
                return nullptr;
            }
            skAnimatedImage->reset();
            return skAnimatedImage;
        };
    }
    executeTaskInBackground(taskId);
}

int ResourceLoader::generateTaskId() {
    return taskID++;
}

void ResourceLoader::executeTaskInBackground(int taskId) {
    env->CallVoidMethod(skiaEngine, executeTaskMethod, taskId);
}

void ResourceLoader::finishTaskInUIThread(JNIEnv *env, int taskId) {
    env->CallVoidMethod(skiaEngine, postTaskMethod, taskId);
}

}
