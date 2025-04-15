#pragma once

#include "Page.h"
#include "FlexboxLayout.h"
#include "effects/SkGradientShader.h"
#include "LinearAnimator.h"

namespace HYSkiaUI {

class BlendTestPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

public:

    class LightenFlexboxLayout : public FlexboxLayout {

    public:

        void layout(int l, int t, int r, int b) override {
            FlexboxLayout::layout(l, t, r, b);
            if (firstFlag) {
                delta = -width;
                firstFlag = false;
                auto animator = new LinearAnimator(this, -width, width);
                animator->setEaseType(EaseType::Linear);
                animator->setLoopCount(-1);
                animator->setDuration(5000);
                animator->setUpdateListener([this](View *view, float value) {
                    this->delta = value;
                });
                animator->start();
            }
        }

        void draw(SkCanvas *canvas) override {
            FlexboxLayout::draw(canvas);
            SkColor colors[] = {
                    SK_ColorTRANSPARENT,
                    SK_ColorMAGENTA,
                    SK_ColorMAGENTA,
                    SK_ColorTRANSPARENT,
            };
            SkPoint horz[] = {
                    {left + this->delta,  static_cast<float >(top)},
                    {right + this->delta, static_cast<float >(top)},
            };
            SkPaint paint;
            paint.setShader(SkGradientShader::MakeLinear(horz, colors, nullptr, std::size(colors),
                                                         SkTileMode::kClamp));
            canvas->save();
            paint.setBlendMode(SkBlendMode::kPlus);
            canvas->drawPaint(paint);
            canvas->restore();
        }

    private:

        bool firstFlag = true;

        float delta = 0.0f;

    };

};

}
