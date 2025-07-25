#include <base/native_log.h>

#include <utility>
#include "core/SkColorFilter.h"
#include "ImageView.h"
#include "core/SkData.h"
#include "SkiaUIContext.h"
#include "effects/SkImageFilters.h"
#include "codec/SkAndroidCodec.h"
#include "LinearAnimator.h"
#include "w3c_util.h"

namespace HYSkiaUI {

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
    imagePaint->setAlphaf(alpha);
    markDirty();
}

float ImageView::getAlpha() {
    return imagePaint->getAlpha();
}

void ImageView::setSource(const char *path) {
    MeasureTime measureTime("ImageView setSource");
    this->source = std::string(path);
    auto resourcesLoader = getContext()->resourcesLoader;
    resourcesLoader->decodeImage(source, [this](sk_sp<SkAnimatedImage> animatedImage) {
        this->skAnimatedImage = std::move(animatedImage);
        skImage = skAnimatedImage->getCurrentFrame();
        srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
        ALOGD("decode image success %s %d %d", source.c_str(), skImage->width(), skImage->height())
        startTime = getContext()->getCurrentTimeMills();
        this->frameCount = skAnimatedImage->getFrameCount();
        this->duration = this->frameCount * skAnimatedImage->currentFrameDuration();
        endTime = startTime + this->duration;
        markDirty();
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
    if (scaleEffectFlag && (!YGFloatsEqual(translateX, 0.0f) || !YGFloatsEqual(translateY, 0.0f))) {
        scaleRect.setLTRB(l + translateX, t + translateY, l + translateX + width * 1.3f,
                          t + translateY + height * 1.3f);
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
        auto index = (currentTimeMills - startTime) * frameCount / duration;
        if (currentFrameIndex != index) {
            currentFrameIndex = index;
            decodeNextFrame();
            if (currentFrameIndex == 0 && completeFunc != nullptr) {
                completeFunc(this);
            }
            skImage = skAnimatedImage->getCurrentFrame();
        }
        markDirty();
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
    if (scaleEffectFlag && (!YGFloatsEqual(translateX, 0.0f) || !YGFloatsEqual(translateY, 0.0f))) {
        canvas->drawImageRect(skImage, scaleRect, SkSamplingOptions(), imagePaint.get());
    }
}

const char *ImageView::name() {
    return "ImageView";
}

void ImageView::setCornerRadius(int radius) {
    this->radius = static_cast<float >(radius);
    View::setCornerRadius(radius);
    markDirty();
}

void ImageView::setScaleType(ImageView::ScaleType scaleType) {
    this->scaleType = scaleType;
    markDirty();
}

void ImageView::setBlur(float blur) {
    auto filter = SkImageFilters::Blur(blur, blur, SkTileMode::kClamp, nullptr);
    imagePaint->setImageFilter(filter);
    markDirty();
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

void ImageView::setOnCompleteFunc(std::function<void(ImageView *imageView)> &&completeFunc) {
    this->completeFunc = std::move(completeFunc);
}

void ImageView::setScaleEffect(bool flag) {
    this->scaleEffectFlag = flag;
}

bool ImageView::onInterceptTouchEvent(TouchEvent *touchEvent) {
    return scaleEffectFlag;
}

bool ImageView::onTouchEvent(TouchEvent *touchEvent) {
    if (!scaleEffectFlag) {
        return false;
    }
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            translateX = 0.0f;
            translateY = 0.0f;
            lastX = touchEvent->x;
            lastY = touchEvent->y;
            break;
        }
        case TouchEvent::ACTION_MOVE: {
            translateX += touchEvent->x - lastX;
            translateY += touchEvent->y - lastY;
            lastX = touchEvent->x;
            lastY = touchEvent->y;
            break;
        }
        case TouchEvent::ACTION_UP: {
            auto animator = new LinearAnimator(this, translateX, 0.0f);
            animator->setDuration(300L);
            auto x = translateX;
            auto y = translateY;
            animator->setUpdateListener([x, y](View *view, float value) {
                view->setTranslateX(value);
                view->setTranslateY(value * y / x);
            });
            animator->start();
            break;
        }
        default: {
            break;
        }
    }
    markDirty();
    return true;
}

void ImageView::setSkImage(sk_sp<SkImage> image) {
    this->skImage = image;
    srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
    markDirty();
}

void ImageView::decodeNextFrame() {
//    MeasureTime measureTime("decodeNextFrame");
    if (skAnimatedImage->decodeNextFrame() == SkAnimatedImage::kFinished) {
        skAnimatedImage->reset();
    }
}

}
