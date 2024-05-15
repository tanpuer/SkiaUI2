#pragma once


#include "View.h"

class ClockView : public View {

public:

    ClockView();

    ~ClockView();

    void draw(SkCanvas *canvas) override;

private:

    void drawBackground(SkCanvas* canvas);

    void drawClockHand(SkCanvas* canvas, float angle, float length, float handWidth, SkColor color);

    std::unique_ptr<SkPaint> numberPaint;

    std::unique_ptr<SkFont> font;

    std::unique_ptr<SkPaint> circlePaint;

    std::unique_ptr<SkPaint> detailPaint;

    std::unique_ptr<SkPaint> centerPaint;

    std::unique_ptr<SkPaint> handPaint;

};
