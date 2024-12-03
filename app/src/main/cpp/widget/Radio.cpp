#include "Radio.h"

namespace HYSkiaUI {

Radio::Radio() {
    radioPaint = std::make_unique<SkPaint>();
    radioPaint->setAntiAlias(true);
    radioPaint->setColor(SK_ColorGRAY);
    setOnClickListener([this](View *view) {
        selected = !selected;
        markDirty();
        if (this->changeFunc) {
            this->changeFunc(selected);
        }
    });
}

Radio::~Radio() {

}

void Radio::setOnChangeListener(std::function<void(bool)> &&changeFunc) {
    this->changeFunc = std::move(changeFunc);
}

void Radio::draw(SkCanvas *canvas) {
    if (selected) {
        radioPaint->setColor(SkColorSetARGB(255, 31, 132, 226));
        radioPaint->setStyle(SkPaint::Style::kStrokeAndFill_Style);
        radioPaint->setStrokeWidth(2);
        canvas->drawCircle(left + width / 2.0, top + height / 2.0, width / 2.0, *radioPaint);
        radioPaint->setColor(SK_ColorWHITE);
        radioPaint->setStyle(SkPaint::Style::kStroke_Style);
        radioPaint->setStrokeWidth(5);
        SkPath path;
        path.moveTo(left + width / 4.0, top + height / 2.0);
        path.lineTo(left + width / 2.0, top + height * 0.7);
        path.lineTo(left + width * 0.8, top + height * 0.3);
        canvas->drawPath(path, *radioPaint);
    } else {
        radioPaint->setColor(SK_ColorGRAY);
        radioPaint->setStyle(SkPaint::Style::kStrokeAndFill_Style);
        radioPaint->setStrokeWidth(2);
        canvas->drawCircle(left + width / 2.0, top + height / 2.0, width / 2.0, *radioPaint);
    }
}

void Radio::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    SkASSERT(width == height);
}

void Radio::setOnClickListener(std::function<void(View *)> clickListener) {
    View::setOnClickListener(clickListener);
}

void Radio::setSelected(bool selected) {
    this->selected = selected;
    markDirty();
}

}
