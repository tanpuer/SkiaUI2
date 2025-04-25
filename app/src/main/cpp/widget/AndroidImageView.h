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

    void setJavaBitmap(JNIEnv *env, jobject bitmap);

    void onShow() override;

    void onHide() override;

    void blur(float blur);

    void setCornerRadius(int radius) override;

    void setRotateFunc(std::function<void(SkRect &, SkMatrix &, float)> &&rotateFunc);

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

};

}
