#pragma once

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
