#include "GradientCanvasTest.h"
#include "effects/SkGradientShader.h"
#include "core/SkMaskFilter.h"

HYSkiaUI::GradientCanvasTest::GradientCanvasTest() {
    redPaint = std::make_unique<SkPaint>();
    redPaint->setAntiAlias(true);
    redPaint->setStyle(SkPaint::Style::kFill_Style);
    redPaint->setColor(SkColorSetA(SK_ColorRED, 200));
    auto filter = SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, 50);
    redPaint->setMaskFilter(filter);

    rightPaint = std::make_unique<SkPaint>();
    rightPaint->setAntiAlias(true);
    rightPaint->setStyle(SkPaint::Style::kFill_Style);
    rightPaint->setColor(SkColorSetA(SK_ColorRED, 200));
    auto filterRight = SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, 50);
    rightPaint->setMaskFilter(filterRight);

    borderPaint = std::make_unique<SkPaint>();
    borderPaint->setAntiAlias(true);
    borderPaint->setStyle(SkPaint::Style::kStroke_Style);
    borderPaint->setColor(SK_ColorRED);
    borderPaint->setStrokeWidth(10);
}

HYSkiaUI::GradientCanvasTest::~GradientCanvasTest() {

}

void HYSkiaUI::GradientCanvasTest::draw(SkCanvas *canvas) {
    canvas->save();
    canvas->clipRRect(rRect, true);
    View::draw(canvas);
    canvas->drawRect(leftRect, *redPaint);
    canvas->drawRect(rightRect, *rightPaint);
    canvas->drawRoundRect(borderRect, 50, 50, *borderPaint);
    canvas->restore();
}

void HYSkiaUI::GradientCanvasTest::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    leftRect.setLTRB(l - 300, b - 300, l + 300, b + 300);
    rightRect.setLTRB(r - height, t - height, r + height, b);
    borderRect.setLTRB(l + 30, t + 30, r - 30, b - 30);
    SkVector corners[] = {{50, 50}, {50, 50}, {50, 50}, {50, 50}};
    rRect.setRectRadii(borderRect, corners);

    const SkColor colors[] = {SK_ColorRED};
    redPaint->setShader(SkGradientShader::MakeSweep(l, b, colors, nullptr, 1));
}
