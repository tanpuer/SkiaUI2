#include <base/native_log.h>

#include <utility>
#include "core/SkColorFilter.h"
#include "ImageView.h"
#include "core/SkData.h"
#include "SkiaUIContext.h"
#include "effects/SkImageFilters.h"
#include "codec/SkAndroidCodec.h"

ImageView::ImageView() : View(), radius(0), scaleType(ScaleType::FitXY) {
    imagePaint = std::make_unique<SkPaint>();
    imagePaint->setAntiAlias(true);
    srcRect.setEmpty();
    dstRect.setEmpty();
    clipRect.setEmpty();
    imageMatrix = SkMatrix::I();
}

ImageView::~ImageView() {
//    ALOGD("~View ImageView")
}

void ImageView::setAlpha(float alpha) {
    SkASSERT(imagePaint);
    imagePaint->setAlphaf(alpha);
}

float ImageView::getAlpha() {
    return imagePaint->getAlpha();
}

void ImageView::setSource(const char *path) {
    MeasureTime measureTime("ImageView setSource");
    this->source = std::string(path);
    auto resourcesLoader = getContext()->resourcesLoader;
    resourcesLoader->decodeImage(source, [this](const std::vector<sk_sp<SkImage>> &images,
                                                sk_sp<SkAnimatedImage> animatedImage) {
        skImage = images[0];
        srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
        ALOGD("decode image success %s %d %d", source.c_str(), skImage->width(), skImage->height())
        startTime = getContext()->getCurrentTimeMills();
        this->skImages = images;
        this->skAnimatedImage = std::move(animatedImage);
        this->frameCount = images.size();
        this->duration = this->frameCount * skAnimatedImage->currentFrameDuration();
        endTime = startTime + this->duration;
        isDirty = true;
    });
}

void ImageView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
    if (skImage == nullptr) {
        return;
    }
    imageMatrix.setIdentity();
    if (scaleType == ScaleType::FitCenter) {
        auto imageRatio =
                static_cast<float>(skImage->width()) / static_cast<float >(skImage->height());
        auto viewRatio = dstRect.width() / dstRect.height();
        if (imageRatio > viewRatio) {
            imageMatrix.preScale(1.0f, viewRatio / imageRatio, dstRect.centerX(),
                                 dstRect.centerY());
        } else {
            imageMatrix.preScale(imageRatio / viewRatio, 1.0f, dstRect.centerX(),
                                 dstRect.centerY());
        }
    } else if (scaleType == ScaleType::CenterCrop) {
        auto imageRatio =
                static_cast<float>(skImage->width()) / static_cast<float >(skImage->height());
        auto viewRatio = dstRect.width() / dstRect.height();
        if (imageRatio > viewRatio) {
            imageMatrix.preScale(imageRatio / viewRatio, 1.0f, dstRect.centerX(),
                                 dstRect.centerY());
        } else {
            imageMatrix.preScale(1.0f, viewRatio / imageRatio, dstRect.centerX(),
                                 dstRect.centerY());
        }
    }
    if (rotateFunc != nullptr) {
        rotateFunc(dstRect, imageMatrix, rotateZ);
    } else {
        imageMatrix.preRotate(rotateZ, dstRect.centerX(), dstRect.centerY());
    }
}

void ImageView::draw(SkCanvas *canvas) {
    if (skImage == nullptr) {
        return;
    }
    View::draw(canvas);
    if (frameCount > 1 && skAnimatedImage != nullptr && autoPlay) {
        auto currentTimeMills = getContext()->getCurrentTimeMills();
        if (currentTimeMills > endTime) {
            startTime = currentTimeMills;
            endTime = startTime + duration;
        }
        currentFrameIndex = (currentTimeMills - startTime) * frameCount / duration;
        if (currentFrameIndex >= skImages.size()) {
            currentFrameIndex = 0;
        }
        skImage = skImages[currentFrameIndex];
    }
    canvas->save();
    clipRect.setRectXY(dstRect, radius, radius);
    //Todo
    if (needClip) {
        canvas->clipRRect(clipRect);
    }
    canvas->setMatrix(imageMatrix);
    canvas->drawImageRect(skImage, srcRect, dstRect, SkSamplingOptions(), imagePaint.get(),
                          SkCanvas::kFast_SrcRectConstraint);
    canvas->restore();
}

const char *ImageView::name() {
    return "ImageView";
}

void ImageView::setCornerRadius(int radius) {
    this->radius = static_cast<float >(radius);
    View::setCornerRadius(radius);
    isDirty = true;
}

void ImageView::setScaleType(ImageView::ScaleType scaleType) {
    this->scaleType = scaleType;
    isDirty = true;
}

void ImageView::blur(float blur) {
    auto filter = SkImageFilters::Blur(blur, blur, SkTileMode::kClamp, nullptr);
    imagePaint->setImageFilter(filter);
    isDirty = true;
}

const char *ImageView::getSource() {
    return source.c_str();
}

const ImageView::ScaleType ImageView::getScaleType() {
    return scaleType;
}

void ImageView::setRotateFunc(std::function<void(SkRect &, SkMatrix &, float)> &&rotateFunc) {
    this->rotateFunc = std::move(rotateFunc);
}

void ImageView::pause() {
    if (!autoPlay) {
        return;
    }
    autoPlay = false;
    pausedTime = getContext()->getCurrentTimeMills();
}

void ImageView::start() {
    if (autoPlay) {
        return;
    }
    autoPlay = true;
    auto diff = getContext()->getCurrentTimeMills() - pausedTime;
    startTime += diff;
    endTime += diff;
}


