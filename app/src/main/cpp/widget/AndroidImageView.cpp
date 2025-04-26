#include "AndroidImageView.h"
#include "android/bitmap.h"
#include "core/SkColorSpace.h"
#include "effects/SkImageFilters.h"

namespace HYSkiaUI {

AndroidImageView::AndroidImageView() {
    imagePaint = std::make_unique<SkPaint>();
    imagePaint->setAntiAlias(true);
    paint->setStyle(SkPaint::Style::kStroke_Style);
    paint->setColor(SK_ColorRED);
    paint->setStrokeWidth(2);
}

AndroidImageView::~AndroidImageView() {
    auto jniEnv = context->getJniEnv();
    if (javaInstance != nullptr) {
        jniEnv->CallVoidMethod(javaInstance, releaseMethodId);
        jniEnv->DeleteGlobalRef(javaInstance);
        javaInstance = nullptr;
    }
}

void AndroidImageView::setSource(const char *source) {
    skImage = nullptr;
    this->source = source;
    auto jniEnv = context->getJniEnv();
    checkInstance();
    auto jSource = jniEnv->NewStringUTF(source);
    jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jSource);
    jniEnv->DeleteLocalRef(jSource);
}

void AndroidImageView::setResId(int resId) {
    skImage = nullptr;
    this->resId = resId;
    auto jniEnv = context->getJniEnv();
    checkInstance();
    jniEnv->CallVoidMethod(javaInstance, setResIdMethodId, resId);
}

void AndroidImageView::setScaleType(ImageView::ScaleType scaleType) {
    this->scaleType = scaleType;
    markDirty();
}

void AndroidImageView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (skImage == nullptr) {
        return;
    }
    imageMatrix.setIdentity();
    if (scaleType == ImageView::ScaleType::FitCenter) {
        auto imageRatio =
                static_cast<float>(skImage->width()) / static_cast<float >(skImage->height());
        auto viewRatio = skRect.width() / skRect.height();
        if (imageRatio > viewRatio) {
            imageMatrix.preScale(1.0f, viewRatio / imageRatio, skRect.centerX(),
                                 skRect.centerY());
        } else {
            imageMatrix.preScale(imageRatio / viewRatio, 1.0f, skRect.centerX(),
                                 skRect.centerY());
        }
    } else if (scaleType == ImageView::ScaleType::CenterCrop) {
        auto imageRatio =
                static_cast<float>(skImage->width()) / static_cast<float >(skImage->height());
        auto viewRatio = skRect.width() / skRect.height();
        if (imageRatio > viewRatio) {
            imageMatrix.preScale(imageRatio / viewRatio, 1.0f, skRect.centerX(),
                                 skRect.centerY());
        } else {
            imageMatrix.preScale(1.0f, viewRatio / imageRatio, skRect.centerX(),
                                 skRect.centerY());
        }
    }
    if (rotateFunc != nullptr) {
        rotateFunc(skRect, imageMatrix, rotateZ);
    } else {
        imageMatrix.preRotate(rotateZ, skRect.centerX(), skRect.centerY());
    }
}

void AndroidImageView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (skImage == nullptr || javaInstance == nullptr) {
        return;
    }
    canvas->save();
    clipRect.setRectXY(skRect, radius, radius);
    canvas->clipRRect(clipRect);
    canvas->setMatrix(imageMatrix);
    canvas->drawImageRect(skImage, srcRect, skRect, SkSamplingOptions(), imagePaint.get(),
                          SkCanvas::kFast_SrcRectConstraint);
    canvas->restore();
}

void AndroidImageView::setJavaBitmap(JNIEnv *env, jobject bitmap, int index, int frameCount) {
    if (javaInstance == nullptr) {
        return;
    }
    AndroidBitmapInfo info;
    void *pixels = nullptr;
    auto res = AndroidBitmap_getInfo(env, bitmap, &info);
    if (res != ANDROID_BITMAP_RESULT_SUCCESS) {
        ALOGE("AndroidBitmap_getInfo error %d", res)
        return;
    }
    res = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (res != ANDROID_BITMAP_RESULT_SUCCESS) {
        ALOGE("AndroidBitmap_lockPixels error %d", res)
        return;
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
    skBitmap = std::make_unique<SkBitmap>();
    skBitmap->setInfo(skInfo, info.stride);
    skBitmap->setPixels(pixels);
    AndroidBitmap_unlockPixels(env, bitmap);
    skImage = skBitmap->asImage();
    srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
    markDirty();
    if (completeFunc != nullptr && frameCount > 0 && index < lastIndex) {
        completeFunc(this);
    }
    lastIndex = index;
}

void AndroidImageView::checkInstance() {
    if (javaInstance == nullptr) {
        auto jniEnv = context->getJniEnv();
        auto javaClass = jniEnv->FindClass("com/temple/skiaui/bitmap/AndroidBitmapLoader");
        auto javaConstructor = jniEnv->GetMethodID(javaClass, "<init>",
                                                   "(Lcom/temple/skiaui/HYSkiaEngine;J)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        javaInstance = jniEnv->NewGlobalRef(
                jniEnv->NewObject(javaClass, javaConstructor, javaSkiaEngine,
                                  reinterpret_cast<long>(this)));
        setSourceMethodId = jniEnv->GetMethodID(javaClass, "setSource", "(Ljava/lang/String;)V");
        releaseMethodId = jniEnv->GetMethodID(javaClass, "release", "()V");
        setResIdMethodId = jniEnv->GetMethodID(javaClass, "setResId", "(I)V");
        startMethodId = jniEnv->GetMethodID(javaClass, "start", "()V");
        stopMethodId = jniEnv->GetMethodID(javaClass, "stop", "()V");
    }
}

void AndroidImageView::onShow() {
    if (userSetPaused) {
        return;
    }
    innerStart();
}

void AndroidImageView::onHide() {
    innerStop();
}

void AndroidImageView::setBlur(float blur) {
    auto filter = SkImageFilters::Blur(blur, blur, SkTileMode::kClamp, nullptr);
    imagePaint->setImageFilter(filter);
    markDirty();
}

void AndroidImageView::setCornerRadius(int radius) {
    this->radius = static_cast<float >(radius);
    View::setCornerRadius(radius);
    markDirty();
}

void
AndroidImageView::setRotateFunc(std::function<void(SkRect &, SkMatrix &, float)> &&rotateFunc) {
    this->rotateFunc = std::move(rotateFunc);
}

void AndroidImageView::setOnCompleteFunc(std::function<void(AndroidImageView *)> &&completeFunc) {
    this->completeFunc = std::move(completeFunc);
}

void AndroidImageView::start() {
    userSetPaused = false;
    innerStart();
}

void AndroidImageView::stop() {
    userSetPaused = true;
    innerStop();
}

void AndroidImageView::innerStart() {
    if (javaInstance == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(javaInstance, startMethodId);
}

void AndroidImageView::innerStop() {
    if (javaInstance == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(javaInstance, stopMethodId);
}

float AndroidImageView::getAlpha() {
    return imagePaint->getAlpha();
}

void AndroidImageView::setAlpha(float alpha) {
    imagePaint->setAlphaf(alpha);
    markDirty();
}

}
