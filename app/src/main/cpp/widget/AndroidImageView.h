#pragma once

#include "View.h"
#include "ImageView.h"

namespace HYSkiaUI {

/**
 * using Glide to decode Android Resources to Bitmap, then using skia to draw
 */
class AndroidImageView : public View {

public:

    AndroidImageView();

    ~AndroidImageView();

    void setSource(const char *source);

    void setResId(int resId);

    void setScaleType(ImageView::ScaleType scaleType);

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void setJavaBitmap(JNIEnv *env, jobject bitmap, int index, int frameCount);

    void onShow() override;

    void onHide() override;

    void setBlur(float blur) override;

    void setCornerRadius(int radius) override;

    void setRotateFunc(std::function<void(SkRect &, SkMatrix &, float)> &&rotateFunc);

    void stop();

    void start();

    void setOnCompleteFunc(std::function<void(AndroidImageView *imageView)> &&completeFunc);

    float getAlpha() override;

    void setAlpha(float alpha) override;

private:

    SkMatrix imageMatrix;

    ImageView::ScaleType scaleType = ImageView::ScaleType::FitCenter;

    SkRect srcRect;

    std::string source;

    int resId = -1;

    std::unique_ptr<SkBitmap> skBitmap = nullptr;

    sk_sp<SkImage> skImage = nullptr;

    jobject javaInstance = nullptr;

    jmethodID setSourceMethodId = nullptr;

    jmethodID releaseMethodId = nullptr;

    jmethodID setResIdMethodId = nullptr;

    jmethodID startMethodId = nullptr;

    jmethodID stopMethodId = nullptr;

    void checkInstance();

    float radius = 0.0f;

    SkRRect clipRect;

    std::function<void(SkRect &dstRect, SkMatrix &imageMatrix, float rotateZ)> rotateFunc = nullptr;

    std::function<void(AndroidImageView *imageView)> completeFunc = nullptr;

    bool userSetPaused = false;

    void innerStart();

    void innerStop();

    int lastIndex = 0;

    std::unique_ptr<SkPaint> imagePaint;

};

}
