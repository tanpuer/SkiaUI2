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

    void setTimeMills(long time) {
        currentTimeMills = time;
    }

    long getCurrentTimeMills() {
        return currentTimeMills;
    }

    void setConfigRef(YGConfigRef config) {
        this->config = config;
    }

    const YGConfigRef getConfig() {
        return config;
    }

private:

    std::shared_ptr<AssetManager> assetManager;

    long currentTimeMills = 0L;

    YGConfigRef config;

};