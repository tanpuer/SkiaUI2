#pragma once

#include "TextView.h"

class Button : public TextView {

public:

    Button();

    ~Button();

    const char *name() override;

    void setMeasuredDimension(int _measuredWidth, int _measuredHeight) override;

    void draw(SkCanvas *canvas) override;

private:

    /**
     * 水波纹
     */
    SkPaint *ripplePaint;

    int defaultRadius = 20;

    int defaultPadding = 20;

};
