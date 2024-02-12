#pragma once

#include "jni.h"
#include "AssetManager.h"
#include "memory"

class SkiaUIContext {

public:

    static SkiaUIContext *getInstance() {
        static SkiaUIContext context;
        return &context;
    }

    void setJavaAssetManager(JNIEnv *env, jobject javaAssetManager) {
        assetManager = std::make_shared<AssetManager>(env, javaAssetManager);
    }

    std::shared_ptr<AssetManager> getAssetManager() {
        return assetManager;
    }

private:

    std::shared_ptr<AssetManager> assetManager;

};