#pragma once

#include "core/SkImage.h"
#include "jni.h"
#include "android/bitmap.h"
#include "native_log.h"
#include "core/SkColorSpace.h"
#include "memory"
#include "core/SkBitmap.h"

namespace HYSkiaUI {

static sk_sp<SkImage> transferBitmapToSkImage(JNIEnv *env, jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels = nullptr;
    auto res = AndroidBitmap_getInfo(env, bitmap, &info);
    if (res != ANDROID_BITMAP_RESULT_SUCCESS) {
        ALOGE("AndroidBitmap_getInfo error %d", res)
        return nullptr;
    }
    res = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (res != ANDROID_BITMAP_RESULT_SUCCESS) {
        ALOGE("AndroidBitmap_lockPixels error %d", res)
        return nullptr;
    }
    SkColorType colorType = kUnknown_SkColorType;
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        colorType = kRGBA_8888_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        colorType = kRGB_565_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_4444) {
        colorType = kARGB_4444_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_A_8) {
        colorType = kAlpha_8_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_F16) {
        colorType = kRGBA_F16_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_1010102) {
        colorType = kRGBA_1010102_SkColorType;
    }
    SkImageInfo skInfo = SkImageInfo::Make(
            static_cast<int>(info.width), static_cast<int>(info.height),
            colorType,
            kPremul_SkAlphaType,
            SkColorSpace::MakeSRGB()
    );
    auto skBitmap = std::make_unique<SkBitmap>();
    skBitmap->setInfo(skInfo, info.stride);
    skBitmap->setPixels(pixels);
    AndroidBitmap_unlockPixels(env, bitmap);
    return skBitmap->asImage();
}

}
