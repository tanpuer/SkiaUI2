#pragma once

#include <unordered_map>
#include "AndroidBitmap.h"

namespace HYSkiaUI {

class AndroidBitmapLoader {

public:

    static AndroidBitmapLoader *getInstance() {
        static AndroidBitmapLoader imageLoader;
        return &imageLoader;
    }

    std::shared_ptr<AndroidBitmap> &
    loadImage(std::shared_ptr<SkiaUIContext> &context, const std::string& url);

    std::shared_ptr<AndroidBitmap> &loadImage(std::shared_ptr<SkiaUIContext> &context, int resId);

    void clear(const std::string& url);

private:

    std::unordered_map<std::string, std::shared_ptr<AndroidBitmap>> activeCache;

};

}
