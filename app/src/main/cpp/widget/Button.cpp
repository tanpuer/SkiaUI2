#include "Button.h"

namespace HYSkiaUI {

Button::Button() : TextView() {
    ripplePaint = new SkPaint();
    ripplePaint->setAntiAlias(true);
    textAlign = TextAlign::kCenter;
}

Button::~Button() {
    delete ripplePaint;
}

void Button::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    TextView::setMeasuredDimension(_measuredWidth + (originWidth > 0 ? 0 : defaultPadding * 2),
                                   _measuredHeight + defaultPadding * 2);
}

void Button::draw(SkCanvas *canvas) {
    if (cornerRadius == 0) {
        setCornerRadius(defaultRadius);
    }
    View::draw(canvas);
    if (paragraph == nullptr) {
        return;
    }
    canvas->save();
    canvas->setMatrix(viewMatrix);
    paragraph->paint(canvas, skRect.left() + (originWidth > 0 ? 0 : defaultPadding),
                     skRect.top() + defaultPadding);
    canvas->restore();
}

const char *Button::name() {
    return "Button";
}

void Button::setContext(std::shared_ptr<SkiaUIContext>& context) {
    View::setContext(context);
    setBackgroundColor(SkColorSetARGB(255, 31, 132, 226));
    setTextColor(SK_ColorWHITE);
}

void Button::setAlpha(float alpha) {
    View::setAlpha(alpha);
}

}
