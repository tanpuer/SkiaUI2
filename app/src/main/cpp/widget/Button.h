#pragma once

#include "TextView.h"

namespace HYSkiaUI {

class Button : public TextView {

public:

    Button();

    ~Button();

    const char *name() override;

    void setMeasuredDimension(int _measuredWidth, int _measuredHeight) override;

    void draw(SkCanvas *canvas) override;

    void setContext(std::shared_ptr<SkiaUIContext> context) override;

    void setAlpha(float alpha) override;

private:

    /**
     * 水波纹
     */
    SkPaint *ripplePaint;

    int defaultRadius = 20;

    int defaultPadding = 20;

};

}
