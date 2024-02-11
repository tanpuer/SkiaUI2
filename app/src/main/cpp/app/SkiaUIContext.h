//
// Created by cw on 2024/2/4.
//

#ifndef SKIAUI_SKIAUICONTEXT_H
#define SKIAUI_SKIAUICONTEXT_H

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


#endif //SKIAUI_SKIAUICONTEXT_H
