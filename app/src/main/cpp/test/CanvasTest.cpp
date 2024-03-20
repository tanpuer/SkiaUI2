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
    pathPaint->setStrokeWidth(2);
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

    canvas->translate(-size / 2, size * 2);
    testIndex++;
    SkPath path;
    SkScalar firstX = left;
    SkScalar firstY = top + 150.0 * sinf((firstX + testIndex) * 0.1);
    path.moveTo(firstX, firstY);
    for (SkScalar x = 1; x < width; x += 2.0f) {
        SkScalar y = top + 150.0 * sinf((left + x + testIndex) * 0.1);
        path.lineTo(left + x, y);
    }
    SkScalar lastX = left + width;
    SkScalar lastY = top + 150.0 * sinf((left + width + testIndex) * 0.1);
    path.lineTo(lastX, lastY);
    SkPoint points[2]{SkPoint::Make(firstX, firstY), SkPoint::Make(lastX, lastY)};
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
