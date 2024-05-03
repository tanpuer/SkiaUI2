#pragma once

#include "jni.h"
#include "AssetManager.h"
#include "memory"
#include "PageStackManager.h"
#include "PluginManager.h"

class SkiaUIContext {

public:

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

    const std::shared_ptr<PageStackManager> getPageStackManager() {
        return pageStackManager;
    }

    const std::shared_ptr<PluginManager> getPluginManager() {
        return pluginManager;
    }

private:

    std::shared_ptr<AssetManager> assetManager;

    long currentTimeMills = 0L;

    YGConfigRef config;

    std::shared_ptr<PageStackManager> pageStackManager = std::make_shared<PageStackManager>();

    std::shared_ptr<PluginManager> pluginManager = std::make_shared<PluginManager>();

};