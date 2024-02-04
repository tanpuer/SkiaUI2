//
// Created by ByteDance on 2022/10/26.
//

#include "Button.h"

Button::Button() : TextView() {
    ripplePaint = new SkPaint();
    ripplePaint->setAntiAlias(true);
}

Button::~Button() {
    delete ripplePaint;
}

void Button::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    TextView::setMeasuredDimension(_measuredWidth + defaultPadding * 2,
                               _measuredHeight + defaultPadding * 2);
}

void Button::draw(SkCanvas *canvas) {
    if (cornerRadius == 0) {
        setCornerRadius(defaultRadius);
    }
    View::draw(canvas);
    SkASSERT(paragraph);
    paragraph->paint(canvas, skRect.left() + defaultPadding, skRect.top() + defaultPadding);
}
