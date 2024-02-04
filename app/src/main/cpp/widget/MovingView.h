//
// Created by ByteDance on 2022/6/22.
//

#ifndef SKIAUI_MOVINGVIEW_H
#define SKIAUI_MOVINGVIEW_H


#include "View.h"

class MovingView : public View {

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


#endif //SKIAUI_MOVINGVIEW_H
