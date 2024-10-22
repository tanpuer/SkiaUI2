#pragma once

#include "View.h"
#include "FlexboxLayout.h"

class MovingView : public FlexboxLayout {

public:

    MovingView();

    virtual ~MovingView();

    virtual void layout(int l, int t, int r, int b) override;

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    const char *name() override;

    virtual bool forceRequestTouchMove() override;

private:

    float lastX;
    float lastY;

};
