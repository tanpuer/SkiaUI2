#include "Switch.h"
#include "LinearAnimator.h"

Switch::Switch() {
    switchPaint = std::make_unique<SkPaint>();
    switchPaint->setAntiAlias(true);
    setOnClickListener([this](View *view) {
        setEnabled(!enable);
    });
    translateX = switchWidth / 2;
}

Switch::~Switch() {

}

void Switch::setColor(SkColor color) {
    switchColor = color;
    isDirty = true;
}

void Switch::setEnabled(bool enable) {
    this->enable = enable;
    auto animator = new LinearAnimator(this, translateX, this->enable ? switchWidth / 2 : 0.0);
    animator->setUpdateListener([this](View *view, float value) -> void {
        this->translateX = value;
    });
    animator->setDuration(switchDuration);
    animator->start();
    isDirty = true;
}

void Switch::measure() {
    setMeasuredDimension(switchWidth, switchHeight);
}

void Switch::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    switchRect.setLTRB(skRect.left(), skRect.top(), skRect.right(), skRect.bottom());
    highlightRect.setLTRB(skRect.left() + distance + translateX,
                          skRect.top() + distance,
                          skRect.left() + skRect.width() / 2 - distance + translateX,
                          skRect.bottom() - distance);
}

void Switch::draw(SkCanvas *canvas) {
    if (enable) {
        switchPaint->setColor(switchColor);
    } else {
        switchPaint->setColor(SK_ColorGRAY);
    }
    switchPaint->setStyle(SkPaint::Style::kStrokeAndFill_Style);
    canvas->drawRoundRect(switchRect, switchRadius, switchRadius, *switchPaint);
    switchPaint->setColor(SK_ColorWHITE);
    canvas->drawRoundRect(highlightRect, switchRadius, switchRadius, *switchPaint);
}

void Switch::setWidth(int width) {
    View::setWidth(width);
}

void Switch::setHeight(int height) {
    View::setHeight(height);
}

void Switch::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    View::setMeasuredDimension(_measuredWidth, _measuredHeight);
}
