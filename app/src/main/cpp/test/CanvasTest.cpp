//
// Created by cw on 2024/3/16.
//

#include "CanvasTest.h"
#include "core/SkPath.h"
#include "effects/SkGradientShader.h"

CanvasTest::CanvasTest() {
    paint = std::make_unique<SkPaint>();
    paint->setAntiAlias(true);
    pathPaint = std::make_unique<SkPaint>();
    pathPaint->setAntiAlias(true);
    pathPaint->setStyle(SkPaint::kStroke_Style);
    pathPaint->setStrokeWidth(20);
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

    canvas->translate(-size, size * 2);
    SkPath path;
    path.moveTo(left, top);
    path.moveTo(left + 50, top);
    path.moveTo(left + 100, top + 80);
    path.lineTo(left + 150, top + 50);
    path.lineTo(left + 200, top + 80);
    path.lineTo(left + 250, top + 120);
    path.lineTo(left + 300, top + 70);
    path.lineTo(left + 350, top + 40);
    path.lineTo(left + 400, top + 100);
    path.lineTo(left + 450, top + 200);
    path.lineTo(left + 500, top + 150);
    path.lineTo(left + 550, top + 40);
    path.lineTo(left + 600, top + 0);
    path.lineTo(left + 650, top - 100);
    path.lineTo(left + 700, top + 0);
    path.lineTo(left + 760, top + 30);
    SkPoint points[2]{SkPoint::Make(left, top), SkPoint::Make(left + 700, top + 30)};

    std::vector<SkColor> colors{SK_ColorGREEN, SK_ColorBLUE, SK_ColorMAGENTA};
    auto gradientShader = SkGradientShader::MakeLinear(
            points,
            colors.data(),
            nullptr,
            colors.size(),
            SkTileMode::kClamp
    );
    pathPaint->setShader(std::move(gradientShader));
    canvas->drawPath(path, *pathPaint);

    canvas->restore();
    View::draw(canvas);
}

void CanvasTest::setCircleSize(int size) {
    this->size = size;
    isDirty = true;
}
