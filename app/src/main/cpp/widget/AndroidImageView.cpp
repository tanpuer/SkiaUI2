#include "AndroidImageView.h"
#include "effects/SkImageFilters.h"
#include "Page.h"
#include "bitmap_util.h"

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
    this->resId = -1;
    this->source = source;
    checkInstance();
    if (width > 0 || height > 0) {
        setSourceJNI();
        return;
    }
    imageUpdatedFlag = true;
}

void AndroidImageView::setResId(int resId) {
    skImage = nullptr;
    this->source = "";
    this->resId = resId;
    checkInstance();
    if (width > 0 || height > 0) {
        setSourceJNI();
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
        setSourceJNI();
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
    skImage = transferBitmapToSkImage(env, bitmap);
    srcRect.setWH(static_cast<float>(skImage->width()), static_cast<float >(skImage->height()));
    markDirty();
    if (completeFunc != nullptr && frameCount > 0 && index < lastIndex) {
        completeFunc(this);
    }
    lastIndex = index;
    auto page = getPage();
    if (page != nullptr && !page->getVisibility()) {
        innerStop();
    }
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
        setSourceMethodId = jniEnv->GetMethodID(javaClass, "setSource", "(Ljava/lang/String;II)V");
        releaseMethodId = jniEnv->GetMethodID(javaClass, "release", "()V");
        setResIdMethodId = jniEnv->GetMethodID(javaClass, "setResId", "(III)V");
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

void AndroidImageView::setSourceJNI() {
    if (resId > 0) {
        auto jniEnv = context->getJniEnv();
        jniEnv->CallVoidMethod(javaInstance, setResIdMethodId, resId, width, height);
    } else if (!source.empty()) {
        auto jniEnv = context->getJniEnv();
        auto jSource = jniEnv->NewStringUTF(source.c_str());
        jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jSource, width, height);
        jniEnv->DeleteLocalRef(jSource);
    }
}

}
