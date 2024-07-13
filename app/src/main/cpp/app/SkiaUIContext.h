#pragma once

#include "jni.h"
#include "AssetManager.h"
#include "memory"
#include "PageStackManager.h"
#include "PluginManager.h"
#include "YGConfig.h"
#include "core/SkCanvas.h"

class SkiaUIContext {

public:

    SkiaUIContext() {

    }

    ~SkiaUIContext() {
        jniEnv->DeleteGlobalRef(javaSkiaEngine);
    }

    void setJavaAssetManager(JNIEnv *env, jobject javaAssetManager) {
        this->jniEnv = env;
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

    JNIEnv *getJniEnv() {
        return jniEnv;
    }

    void setJavaSkiaEngine(jobject instance) {
        javaSkiaEngine = jniEnv->NewGlobalRef(instance);
    }

    const jobject getJavaSkiaEngine() {
        return javaSkiaEngine;
    }

    void setCanvas(SkCanvas *canvas) {
        this->canvas = canvas;
    }

    SkCanvas *getCanvas() const{
        return canvas;
    }

private:

    std::shared_ptr<AssetManager> assetManager;

    long currentTimeMills = 0L;

    YGConfigRef config;

    std::shared_ptr<PageStackManager> pageStackManager = std::make_shared<PageStackManager>();

    std::shared_ptr<PluginManager> pluginManager = std::make_shared<PluginManager>();

    JNIEnv *jniEnv = nullptr;

    jobject javaSkiaEngine = nullptr;

    SkCanvas *canvas;

};