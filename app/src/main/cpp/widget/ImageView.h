#pragma once

#include "View.h"
#include "core/SkImage.h"
#include "android/SkAnimatedImage.h"

namespace HYSkiaUI {

class ImageView : public View {

public:

    enum class ScaleType {
        FitXY,
        FitCenter,
        CenterCrop
    };

public:

    ImageView();

    virtual ~ImageView();

    void layout(int l, int t, int r, int b) override;

    virtual void draw(SkCanvas *canvas) override;

    virtual const char *name() override;

    virtual void setAlpha(float alpha) override;

    float getAlpha() override;

    virtual void setCornerRadius(int radius) override;

#pragma mark ImageView api

    virtual void setSource(const char *path);

    virtual const char *getSource();

    virtual void setScaleType(ScaleType scaleType);

    virtual const ScaleType getScaleType();

    virtual void blur(float blur);

    void setRotateFunc(std::function<void(SkRect &dstRect, SkMatrix &imageMatrix,
                                          float rotateZ)> &&rotateFunc);

    bool needClip = true;

    void pause();

    void start();

    void setOnCompleteFunc(std::function<void(ImageView *imageView)> &&completeFunc);

    void setScaleEffect(bool flag);

    bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

    bool onTouchEvent(TouchEvent *touchEvent) override;

    void setSkImage(sk_sp<SkImage> image);

private:

    sk_sp<SkImage> skImage;

    std::vector<sk_sp<SkImage>> skImages;

    int currentFrameIndex = 0;

    sk_sp<SkAnimatedImage> skAnimatedImage;

    /**
     * >1 for gif
     */
    int frameCount = 1;

    long startTime = 0L;

    long duration = 0L;

    long endTime = 0L;

    bool autoPlay = true;

    long pausedTime = 0L;

    SkRect srcRect;

    SkRect dstRect;

    SkRRect clipRect;

    std::unique_ptr<SkPaint> imagePaint;

    SkMatrix imageMatrix;

    float radius;

    ScaleType scaleType;

    std::string source;

    std::function<void(SkRect &dstRect, SkMatrix &imageMatrix, float rotateZ)> rotateFunc = nullptr;

    std::function<void(ImageView *imageView)> completeFunc = nullptr;

    bool scaleEffectFlag = false;

    float lastX = 0.0f;
    float lastY = 0.0f;

    SkRect scaleRect;

};

}
