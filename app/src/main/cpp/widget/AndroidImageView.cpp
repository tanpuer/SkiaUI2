#include "AndroidImageView.h"
#include "effects/SkImageFilters.h"
#include "Page.h"
#include "bitmap/AndroidBitmapLoader.h"

namespace HYSkiaUI {

AndroidImageView::AndroidImageView() {
    imagePaint = std::make_unique<SkPaint>();
    imagePaint->setAntiAlias(true);
    paint->setStyle(SkPaint::Style::kStroke_Style);
    paint->setColor(SK_ColorRED);
    paint->setStrokeWidth(2);
}

AndroidImageView::~AndroidImageView() {
    if (androidBitmap) {
        clearCallback();
        androidBitmap->unRef();
        androidBitmap = nullptr;
    }
}

void AndroidImageView::setSource(const char *source) {
    if (androidBitmap) {
        clearCallback();
        androidBitmap->unRef();
        androidBitmap = nullptr;
    }
    skImage = nullptr;
    androidBitmap = AndroidBitmapLoader::getInstance()->loadImage(context, source);
    androidBitmap->ref();
    setCallback();
}

void AndroidImageView::setResId(int resId) {
    if (androidBitmap) {
        clearCallback();
        androidBitmap->unRef();
        androidBitmap = nullptr;
    }
    skImage = nullptr;
    androidBitmap = AndroidBitmapLoader::getInstance()->loadImage(context, resId);
    androidBitmap->ref();
    setCallback();
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
            imageMatrix.preScale(1.0f, viewRatio / imageRatio, skRect.centerX(), skRect.centerY());
        } else {
            imageMatrix.preScale(imageRatio / viewRatio, 1.0f, skRect.centerX(), skRect.centerY());
        }
    } else if (scaleType == ImageView::ScaleType::CenterCrop) {
        auto imageRatio =
                static_cast<float>(skImage->width()) / static_cast<float >(skImage->height());
        auto viewRatio = skRect.width() / skRect.height();
        if (imageRatio > viewRatio) {
            imageMatrix.preScale(imageRatio / viewRatio, 1.0f, skRect.centerX(), skRect.centerY());
        } else {
            imageMatrix.preScale(1.0f, viewRatio / imageRatio, skRect.centerX(), skRect.centerY());
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
    if (skImage == nullptr) {
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

void AndroidImageView::onShow() {
    if (userSetPaused) {
        return;
    }
    setCallback();
}

void AndroidImageView::onHide() {
    clearCallback();
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
    setCallback();
}

void AndroidImageView::stop() {
    userSetPaused = true;
    clearCallback();
}

float AndroidImageView::getAlpha() {
    return imagePaint->getAlpha();
}

void AndroidImageView::setAlpha(float alpha) {
    imagePaint->setAlphaf(alpha);
    markDirty();
}

void AndroidImageView::setCallback() {
    if (callbackId >= 0) {
        return;
    }
    callbackId = androidBitmap->setCallback(
            [this](sk_sp<SkImage> image, int index, int frameCount) {
                skImage = image;
                srcRect.setWH(static_cast<float>(skImage->width()),
                              static_cast<float >(skImage->height()));
                markDirty();
                if (completeFunc != nullptr && frameCount > 0 && index < lastIndex) {
                    completeFunc(this);
                }
                lastIndex = index;
            });
}

void AndroidImageView::clearCallback() {
    if (callbackId >= 0) {
        androidBitmap->clearCallback(callbackId);
        callbackId = -1;
    }
}

}
