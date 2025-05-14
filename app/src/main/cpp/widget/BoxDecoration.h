#pragma once

#include "core/SkPaint.h"
#include "core/SkImage.h"
#include "core/SkCanvas.h"

namespace HYSkiaUI {

/**
 * draw sequence:
 * 1. box-shadow
 * 2. background-color
 * 3. background-image
 * 4. border-radius
 */
class BoxDecoration {

public:

    BoxDecoration();

    ~BoxDecoration();

public:

    void setLefTopRadius(float radius);

    void setLeftBottomRadius(float radius);

    void setRightTopRadius(float radius);

    void setRightBottomRadius(float radius);

    void setBgImage(const char *image);

    void setBgColor(SkColor color);

    void setBgBledMode(SkBlendMode mode);

    void setShadow(float sigma);

    void draw(SkCanvas *canvas, SkRect &rect);

protected:

    float leftTopRadius = 0.0f;
    float leftBottomRadius = 0.0f;
    float rightTopRadius = 0.0f;
    float rightBottomRadius = 0.0f;

    sk_sp<SkImage> bgImage = nullptr;

    SkColor bgColor = SK_ColorTRANSPARENT;

    float shadowSigma = 0.f;

    std::unique_ptr<SkPaint> shadowPaint;
    std::unique_ptr<SkPaint> bgColorPaint;
    std::unique_ptr<SkPaint> bgImagePaint;
    std::unique_ptr<SkPaint> radiusPaint;

};

}
