#include <base/native_log.h>
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
    View::setAlpha(alpha);
    SkASSERT(imagePaint);
    imagePaint->setAlphaf(alpha);
}

void ImageView::setSource(const char *path) {
    this->source = std::string(path);
    //todo 异步处理
    auto assetManager = getContext()->getAssetManager();
    auto imageData = assetManager->readImage(path);
    auto length = imageData->length;
    auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
    auto androidCodec = SkAndroidCodec::MakeFromData(skData);
    skAnimatedImage = SkAnimatedImage::Make(std::move(androidCodec));
    frameCount = skAnimatedImage->getFrameCount();
    ALOGD("animated info : %d %d %d", skAnimatedImage->getFrameCount(),
          skAnimatedImage->getRepetitionCount(), skAnimatedImage->currentFrameDuration())
    auto frame = skAnimatedImage->getCurrentFrame();
    skImage = frame;
    lastTimeMills = getContext()->getCurrentTimeMills();
    if (skAnimatedImage->currentFrameDuration() > 0) {
        currentFrameDuration = skAnimatedImage->currentFrameDuration();
    }
    if (skImage == nullptr) {
        ALOGE("skImage is null, pls check %s", path)
        return;
    }
    skImages.push_back(skImage);
    for (int i = 1; i < frameCount; ++i) {
        skAnimatedImage->decodeNextFrame();
        skImages.push_back(skAnimatedImage->getCurrentFrame());
    }
    srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
    ALOGD("decode image success %s %d %d", path, skImage->width(), skImage->height())
    isDirty = true;
}

void ImageView::measure() {
    if (skImage == nullptr) {
        return;
    }
    if (isDirty) {
        if (this->width > 0 && this->height > 0) {
            return;
        }
        setMeasuredDimension(skImage->width(), skImage->height());
    }
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
}

void ImageView::draw(SkCanvas *canvas) {
    if (width == 0 || height == 0) {
        ALOGE("ignore ImageView draw, pls check width and height %d %d", width, height)
        return;
    }
    View::draw(canvas);
    if (frameCount > 1 && skAnimatedImage != nullptr) {
        auto currentTimeMills = getContext()->getCurrentTimeMills();
        if ((currentTimeMills - lastTimeMills) > currentFrameDuration) {
            currentFrameIndex++;
            if (currentFrameIndex >= skImages.size()) {
                currentFrameIndex = 0;
            }
            skImage = skImages[currentFrameIndex];
        }
    }
    canvas->save();
    clipRect.setRectXY(dstRect, radius, radius);
    canvas->clipRRect(clipRect);
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


