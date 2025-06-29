#include "AndroidBitmapLoader.h"

namespace HYSkiaUI {

std::shared_ptr<AndroidBitmap> &
AndroidBitmapLoader::loadImage(std::shared_ptr<SkiaUIContext> &context, const std::string &url) {
    if (activeCache.find(url) == activeCache.end()) {
        auto androidBitmap = std::make_shared<AndroidBitmap>(context);
        activeCache.emplace(url, androidBitmap);
        androidBitmap->setSource(url.c_str());
        androidBitmap->decode(0, 0);
    }
    return activeCache[url];
}

std::shared_ptr<AndroidBitmap> &
AndroidBitmapLoader::loadImage(std::shared_ptr<SkiaUIContext> &context, int resId) {
    auto url = std::to_string(resId);
    if (activeCache.find(url) == activeCache.end()) {
        auto androidBitmap = std::make_shared<AndroidBitmap>(context);
        activeCache.emplace(url, androidBitmap);
        androidBitmap->setResId(resId);
        androidBitmap->decode(0, 0);
    }
    return activeCache[url];
}

void AndroidBitmapLoader::clear(const std::string& url) {
    if (activeCache.find(url) != activeCache.end()) {
        auto bitmap = activeCache[url];
        activeCache.erase(url);
    }
}

}
