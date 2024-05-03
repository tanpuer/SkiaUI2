#include "Button.h"

Button::Button() : TextView() {
    ripplePaint = new SkPaint();
    ripplePaint->setAntiAlias(true);
}

Button::~Button() {
    delete ripplePaint;
}

void Button::setContext(std::shared_ptr<SkiaUIContext> context) {
    View::setContext(context);
    setPadding({defaultPadding, defaultPadding, defaultPadding});
}

void Button::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    TextView::setMeasuredDimension(_measuredWidth, _measuredHeight);
}

void Button::draw(SkCanvas *canvas) {
    if (cornerRadius == 0) {
        setCornerRadius(defaultRadius);
    }
    View::draw(canvas);
    SkASSERT(paragraph);
    paragraph->paint(canvas, skRect.left(), skRect.top() + defaultPadding);
}
