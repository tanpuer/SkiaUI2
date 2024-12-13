#include "MovingView.h"
#include "LinearAnimator.h"
#include "yoga/Utils.h"

namespace HYSkiaUI {

bool MovingView::onTouchEvent(TouchEvent *touchEvent) {
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            translateX = 0.0f;
            translateY = 0.0f;
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
        case TouchEvent::ACTION_UP: {
            auto animator = new LinearAnimator(this, translateX, 0.0f);
            animator->setDuration(500L);
            auto x = translateX;
            auto y = translateY;
            animator->setUpdateListener([x, y](View *view, float value) {
                view->translateX = value;
                view->translateY = value * y / x;
            });
            animator->start();
            break;
        }
        default: {
            break;
        }
    }
    markDirty();
    return true;
}

void MovingView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    skRect.setLTRB(l + translateX, t + translateY, r + translateX, b + translateY);
}

MovingView::MovingView() : FlexboxLayout() {
}

MovingView::~MovingView() {
}

const char *MovingView::name() {
    return "MovingView";
}

bool MovingView::onInterceptTouchEvent(TouchEvent *touchEvent) {
    return true;
}

}
