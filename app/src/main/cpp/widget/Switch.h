#pragma once

#include "View.h"

class Switch : public View {

public:

    Switch();

    ~Switch();

    void setColor(SkColor color);

    void setEnabled(bool enable);

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

private:

    bool enable = true;

    std::unique_ptr<SkPaint> switchPaint = nullptr;

    SkColor switchColor = SkColorSetARGB(255, 31, 132, 226);

    SkRect switchRect;

    SkRect highlightRect;

    float distance = 3.0f;

    void setWidth(int width) override;

    void setHeight(int height) override;

    void setMeasuredDimension(int _measuredWidth, int _measuredHeight) override;

    float translateX = 0.0f;

    int switchWidth = 260;

    int switchHeight = 80;

    int switchRadius = 12;

    int switchDuration = 200;

};