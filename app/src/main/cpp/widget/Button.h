//
// Created by ByteDance on 2022/10/26.
//

#ifndef SKIAUI_BUTTON_H
#define SKIAUI_BUTTON_H


#include "TextView.h"

class Button : public TextView {

public:

    Button();

    ~Button();

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


#endif //SKIAUI_BUTTON_H
