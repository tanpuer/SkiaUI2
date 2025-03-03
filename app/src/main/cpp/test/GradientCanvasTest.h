#pragma once

#include "View.h"

namespace HYSkiaUI {

class GradientCanvasTest : public View {

public:

    GradientCanvasTest();

    ~GradientCanvasTest();

    void draw(SkCanvas *canvas) override;

    void layout(int l, int t, int r, int b) override;

private:

    std::unique_ptr<SkPaint> redPaint;
    std::unique_ptr<SkPaint> rightPaint;
    std::unique_ptr<SkPaint> blurPaint;
    std::unique_ptr<SkPaint> borderPaint;

    SkRect leftRect;
    SkRect rightRect;
    SkRect borderRect;
    SkRRect rRect;

};

}
