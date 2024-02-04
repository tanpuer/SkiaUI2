//
// Created by cw404021@alibaba-inc.com on 2023/11/27.
//

#include "AssetManager.h"

char *AssetManager::readFile(const char *path) {
    AAsset *asset = AAssetManager_open(asset_manager, path, AASSET_MODE_BUFFER);
    off_t fileLength = AAsset_getLength(asset);
    char *fileContent = new char[fileLength + 1];
    AAsset_read(asset, fileContent, fileLength);
    fileContent[fileLength] = '\0';
    AAsset_close(asset);
    return fileContent;
}

AssetManager::AssetManager(JNIEnv *env, jobject javaAssetManager) {
    asset_manager = AAssetManager_fromJava(env, javaAssetManager);
}

ImageData *AssetManager::readImage(const char *path) {
    AAsset *asset = AAssetManager_open(asset_manager, path, AASSET_MODE_BUFFER);
    off_t fileLength = AAsset_getLength(asset);
    auto fileContent = new unsigned char[fileLength + 1];
    AAsset_read(asset, fileContent, fileLength);
    fileContent[fileLength] = '\0';
    AAsset_close(asset);
    auto imageData = new ImageData();
    imageData->length = fileLength;
    imageData->content = fileContent;
    return imageData;
}
