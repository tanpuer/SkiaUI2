#include "MovingArea.h"
#include "LinearAnimator.h"

MovingArea::MovingArea() : FlexboxLayout() {

}

MovingArea::~MovingArea() {

}

bool MovingArea::onInterceptTouchEvent(TouchEvent *touchEvent) {
    return true;
}

bool MovingArea::onTouchEvent(TouchEvent *touchEvent) {
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            lastX = touchEvent->x;
            lastY = touchEvent->y;
            break;
        }
        case TouchEvent::ACTION_MOVE: {
            translateX += touchEvent->x - lastX;
            translateY += touchEvent->y - lastY;
            lastX = touchEvent->x;
            lastY = touchEvent->y;
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

void MovingArea::layout(int l, int t, int r, int b) {
    FlexboxLayout::layout(l + translateX, t + translateY, r + translateX, b + translateY);
}

const char *MovingArea::name() {
    return "MovingArea";
}
