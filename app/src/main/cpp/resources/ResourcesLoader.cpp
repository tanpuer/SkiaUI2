#include "ResourcesLoader.h"
#include "codec/SkAndroidCodec.h"
#include "android/SkAnimatedImage.h"
#include "native_log.h"
#include "AssetManager.h"
#include "core/SkStream.h"

namespace HYSkiaUI {

void ResourcesLoader::decodeImage(
        const std::string &path,
        std::function<void(std::vector<sk_sp<SkImage>>, sk_sp<SkAnimatedImage>)> &&callback) {
    auto taskId = TASK_ID++;
    imagesCallback[taskId] = std::move(callback);
    pathMap[taskId] = path;
    jniEnv->CallVoidMethod(javaSkiaEngine, executeTaskMethod, taskId);
}

void ResourcesLoader::decodeLottie(
        const std::string &path,
        std::function<void(sk_sp<skottie::Animation>)> &&callback) {
    auto taskId = TASK_ID++;
    lottieCallback[taskId] = std::move(callback);
    pathMap[taskId] = path;
    jniEnv->CallVoidMethod(javaSkiaEngine, executeTaskMethod, taskId);
}

void ResourcesLoader::decodeSVG(
        const std::string &path,
        std::function<void(sk_sp<SkSVGDOM>)> &&callback) {
    auto taskId = TASK_ID++;
    svgCallback[taskId] = std::move(callback);
    pathMap[taskId] = path;
    jniEnv->CallVoidMethod(javaSkiaEngine, executeTaskMethod, taskId);
}


void ResourcesLoader::readFile(
        const std::string &path,
        std::function<void(const char *)> &&callback) {
    auto taskId = TASK_ID++;
    fileCallback[taskId] = std::move(callback);
    pathMap[taskId] = path;
    jniEnv->CallVoidMethod(javaSkiaEngine, executeTaskMethod, taskId);
}

void ResourcesLoader::executeTask(JNIEnv *env, int taskId, jobject javaAssets) {
    if (imagesCallback.find(taskId) != imagesCallback.end()) {
        auto path = pathMap[taskId];
        auto assetManager = AssetManager(env, javaAssets);
        auto imageData = assetManager.readImage(path.c_str());
        auto length = imageData->length;
        auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
        auto androidCodec = SkAndroidCodec::MakeFromData(skData);
        auto skAnimatedImage = SkAnimatedImage::Make(std::move(androidCodec));
        auto frameCount = skAnimatedImage->getFrameCount();
        ALOGD("animated info : %d %d %d", skAnimatedImage->getFrameCount(),
              skAnimatedImage->getRepetitionCount(), skAnimatedImage->currentFrameDuration())
        auto frame = skAnimatedImage->getCurrentFrame();
        if (skAnimatedImage->currentFrameDuration() > 0) {
            auto currentFrameDuration = skAnimatedImage->currentFrameDuration();
        }
        if (frame == nullptr) {
            ALOGE("skImage is null, pls check %s", path.c_str())
            return;
        }
        std::vector<sk_sp<SkImage>> skImages;
        skImages.push_back(frame);
        for (int i = 1; i < frameCount; ++i) {
            skAnimatedImage->decodeNextFrame();
            skImages.push_back(skAnimatedImage->getCurrentFrame());
        }
        imagesMap[taskId] = skImages;
        skAnimatedImage->reset();
        animatedImagesMap[taskId] = skAnimatedImage;
        env->CallVoidMethod(javaSkiaEngine, postTaskMethod, taskId);
    } else if (lottieCallback.find(taskId) != lottieCallback.end()) {
        auto path = pathMap[taskId];
        auto assetManager = AssetManager(env, javaAssets);
        auto imageData = assetManager.readImage(path.c_str());
        auto length = imageData->length;
        auto lottieAnimation = skottie::Animation::Make(
                reinterpret_cast<const char *>(imageData->content), length);
        lottieMap[taskId] = lottieAnimation;
        env->CallVoidMethod(javaSkiaEngine, postTaskMethod, taskId);
    } else if (svgCallback.find(taskId) != svgCallback.end()) {
        auto path = pathMap[taskId];
        auto assetManager = AssetManager(env, javaAssets);
        auto imageData = assetManager.readImage(path.c_str());
        auto length = imageData->length;
        auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
        auto stream = SkMemoryStream::Make(skData);
        auto skSVGDom = SkSVGDOM::Builder().make(*stream);
        svgMap[taskId] = skSVGDom;
        env->CallVoidMethod(javaSkiaEngine, postTaskMethod, taskId);
    } else if (fileCallback.find(taskId) != fileCallback.end()) {
        auto path = pathMap[taskId];
        auto assetManager = AssetManager(env, javaAssets);
        auto data = assetManager.readFile(path.c_str());
        fileMap[taskId] = data;
        env->CallVoidMethod(javaSkiaEngine, postTaskMethod, taskId);
    }
}

void ResourcesLoader::postTask(JNIEnv *env, int taskId) {
    if (imagesCallback.find(taskId) != imagesCallback.end()) {
        auto callback = imagesCallback[taskId];
        auto images = imagesMap[taskId];
        auto animatedImage = animatedImagesMap[taskId];
        callback(images, animatedImage);
        imagesCallback.erase(taskId);
        imagesMap.erase(taskId);
        animatedImagesMap.erase(taskId);
        pathMap.erase(taskId);
    } else if (lottieCallback.find(taskId) != lottieCallback.end()) {
        auto callback = lottieCallback[taskId];
        auto lottie = lottieMap[taskId];
        callback(lottie);
        lottieCallback.erase(taskId);
        lottieMap.erase(taskId);
        pathMap.erase(taskId);
    } else if (svgMap.find(taskId) != svgMap.end()) {
        auto callback = svgCallback[taskId];
        auto svg = svgMap[taskId];
        callback(svg);
        svgCallback.erase(taskId);
        svgMap.erase(taskId);
        pathMap.erase(taskId);
    } else if (fileMap.find(taskId) != fileMap.end()) {
        auto callback = fileCallback[taskId];
        auto data = fileMap[taskId];
        callback(data);
        fileCallback.erase(taskId);
        fileMap.erase(taskId);
        pathMap.erase(taskId);
    }
}

}
