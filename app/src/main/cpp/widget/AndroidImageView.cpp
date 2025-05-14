#include "AndroidImageView.h"
#include "effects/SkImageFilters.h"
#include "Page.h"

namespace HYSkiaUI {

AndroidImageView::AndroidImageView() {
    imagePaint = std::make_unique<SkPaint>();
    imagePaint->setAntiAlias(true);
    paint->setStyle(SkPaint::Style::kStroke_Style);
    paint->setColor(SK_ColorRED);
    paint->setStrokeWidth(2);
}

AndroidImageView::~AndroidImageView() {
}

void AndroidImageView::setSource(const char *source) {
    skImage = nullptr;
    androidBitmap->setSource(source);
    if (width > 0 || height > 0) {
        androidBitmap->decode(width, height);
        return;
    }
    imageUpdatedFlag = true;
}

void AndroidImageView::setResId(int resId) {
    skImage = nullptr;
    androidBitmap->setResId(resId);
    if (width > 0 || height > 0) {
        androidBitmap->decode(width, height);
        return;
    }
    imageUpdatedFlag = true;
}

void AndroidImageView::setScaleType(ImageView::ScaleType scaleType) {
    this->scaleType = scaleType;
    markDirty();
}

void AndroidImageView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (imageUpdatedFlag && width > 0 && height > 0) {
        imageUpdatedFlag = false;
        androidBitmap->decode(width, height);
    }
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
    androidBitmap->start();
}

void AndroidImageView::onHide() {
    androidBitmap->stop();
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
    androidBitmap->start();
}

void AndroidImageView::stop() {
    userSetPaused = true;
    androidBitmap->stop();
}

float AndroidImageView::getAlpha() {
    return imagePaint->getAlpha();
}

void AndroidImageView::setAlpha(float alpha) {
    imagePaint->setAlphaf(alpha);
    markDirty();
}

void AndroidImageView::setContext(std::shared_ptr<SkiaUIContext> &context) {
    View::setContext(context);
    androidBitmap = std::make_unique<AndroidBitmap>(context);
    androidBitmap->setCallback([this](sk_sp<SkImage> image, int index, int frameCount) {
        skImage = image;
        srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
        markDirty();
        if (completeFunc != nullptr && frameCount > 0 && index < lastIndex) {
            completeFunc(this);
        }
        lastIndex = index;
        auto page = getPage();
        if (page != nullptr && !page->getVisibility()) {
            androidBitmap->stop();
        }
    });
}

}
