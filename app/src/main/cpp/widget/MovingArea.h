#pragma once

#include "FlexboxLayout.h"

class MovingArea : public FlexboxLayout {

public:

    MovingArea();

    ~MovingArea();

    bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

    bool onTouchEvent(TouchEvent *touchEvent) override;

    void layout(int l, int t, int r, int b) override;

    const char * name() override;

private:

    float lastX = 0.0f;
    float lastY = 0.0f;

};
