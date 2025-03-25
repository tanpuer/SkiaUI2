#include "BlendTestView.h"
#include "effects/SkGradientShader.h"

namespace HYSkiaUI {


BlendTestView::BlendTestView() {
    testPaint = std::make_unique<SkPaint>();
    testPaint->setAntiAlias(true);
    discP = std::make_unique<SkPaint>();
    discP->setAntiAlias(true);
    discP->setBlendMode(SkBlendMode::kPlus);
}

BlendTestView::~BlendTestView() {

}

void BlendTestView::draw(SkCanvas *canvas) {
    testPaint->setBlendMode(mode);
    canvas->save();
    canvas->clipRect(testRect);
    canvas->drawPaint(*paint);
    auto count = canvas->saveLayer(testRect, testPaint.get());

    discP->setColor(SK_ColorCYAN);
    canvas->drawCircle(left + size, top + size, size, *discP);
    canvas->translate(size, 0);
    discP->setColor(SK_ColorMAGENTA);
    canvas->drawCircle(left + size, top + size, size, *discP);
    canvas->translate(-size / 2, size);
    discP->setColor(SK_ColorYELLOW);
    canvas->drawCircle(left + size, top + size, size, *discP);

    canvas->restoreToCount(count);
    canvas->restore();
}

void BlendTestView::setBlendMode(SkBlendMode mode) {
    this->mode = mode;
    markDirty();
}

void BlendTestView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    testRect.setLTRB(l, t, r, b);
    SkColor radColors[] = {0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF};
    auto centerX = left + width / 2;
    auto centerY = top + height / 2;
    auto radius = width / 2;
    auto rad = SkGradientShader::MakeRadial(SkPoint::Make(centerX, centerY), radius, radColors,
                                            nullptr, 3,
                                            SkTileMode::kClamp);

    SkMatrix rotMtx;
    rotMtx.setRotate(-90, centerX, centerY);
    SkColor sweepColors[] = {0xFFFF00FF, 0xFFFF0000, 0xFFFFFF00, 0xFF00FF00,
                             0xFF00FFFF, 0xFF0000FF, 0xFFFF00FF};
    auto sweep = SkGradientShader::MakeSweep(centerX, centerY, sweepColors, nullptr, 7, 0, &rotMtx);
    auto comp = SkShaders::Blend(SkBlendMode::kModulate, std::move(rad), std::move(sweep));
    paint->setShader(std::move(comp));
}

}
