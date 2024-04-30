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

};
