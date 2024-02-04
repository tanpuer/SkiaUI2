//
// Created by cw on 1/26/22.
//

#ifndef SKIAUI_IMAGEVIEW_H
#define SKIAUI_IMAGEVIEW_H


#include "View.h"
#include "core/SkImage.h"

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

    void measure(int widthMeasureSpec, int heightMeasureSpec) override;

    void layout(int l, int t, int r, int b) override;

    virtual void draw(SkCanvas *canvas) override;

    virtual const char *name() override;

    virtual void setAlpha(float alpha) override;

    virtual void setCornerRadius(int radius) override;

#pragma mark ImageView api

    virtual void setSource(const char *path);

    virtual void setScaleType(ScaleType scaleType);

private:

    sk_sp<SkImage> skImage;

    SkRect srcRect;

    SkRect dstRect;

    SkRRect clipRect;

    std::unique_ptr<SkPaint> imagePaint;

    SkMatrix imageMatrix;
    
    float radius;

    ScaleType scaleType;

};


#endif //SKIAUI_IMAGEVIEW_H
