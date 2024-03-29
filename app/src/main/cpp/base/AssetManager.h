//
// Created by cw404021@alibaba-inc.com on 2023/11/27.
//

#ifndef SKIAUI_ASSETMANAGER_H
#define SKIAUI_ASSETMANAGER_H


#include <android/asset_manager_jni.h>

struct ImageData {
    const unsigned char *content;
    off_t length;
};

class AssetManager {

public:

    AssetManager(JNIEnv *env, jobject javaAssetManager);


    char *readFile(const char *path);

    ImageData *readImage(const char *path);

private:

    AAssetManager *asset_manager = nullptr;

};


#endif //SKIAUI_ASSETMANAGER_H
