//
// Created by cw on 2024/3/16.
//

#include "CanvasTest.h"

CanvasTest::CanvasTest() {
    paint = std::make_unique<SkPaint>();
    paint->setAntiAlias(true);
}

CanvasTest::~CanvasTest() {
}

void CanvasTest::draw(SkCanvas *canvas) {
    paint->setBlendMode(SkBlendMode::kMultiply);
    paint->setColor(SK_ColorCYAN);
    canvas->drawCircle(left + size, top + size, size, *paint);
    canvas->save();
    canvas->translate(size, 0);
    paint->setColor(SK_ColorMAGENTA);
    canvas->drawCircle(left + size, top + size, size, *paint);
    canvas->translate(-size / 2, size);
    paint->setColor(SK_ColorYELLOW);
    canvas->drawCircle(left + size, top + size, size, *paint);
    canvas->restore();
    paint->setColor(SK_ColorWHITE);
    View::draw(canvas);
}

void CanvasTest::setCircleSize(int size) {
    this->size = size;
    isDirty = true;
}
