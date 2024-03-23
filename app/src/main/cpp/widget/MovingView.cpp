//
// Created by ByteDance on 2022/6/22.
//

#include "MovingView.h"
#include "LinearAnimator.h"

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
            //松手后做个动画回到原处
            animator = std::make_unique<LinearAnimator>(translateX, translateY);
            animator->setDuration(500L);
            animator->start();
        }
        default: {
            translateX = 0.0f;
            translateY = 0.0f;
            lastX = 0.0f;
            lastY = 0.0f;
            break;
        }
    }
    return true;
}

void MovingView::layout(int l, int t, int r, int b) {
    if (YGFloatsEqual(translateX, 0.0f) && YGFloatsEqual(translateY, 0.0f)) {
        View::layout(l, t, r, b);
    } else {
        ALOGD("MovingView %f %f", translateX, translateY)
        skRect.setLTRB(l + translateX, t + translateY, r + translateX, b + translateY);
        width = r - l;
        height = b - t;
    }
    if (animator != nullptr) {
        if (animator->isEnd()) {
            animator.reset();
        } else {
            AnimationResult result;
            animator->update(skRect, result);
        }
    }
}

MovingView::MovingView() : View() {
    lastX = 0.0f;
    lastY = 0.0f;
}

MovingView::~MovingView() {
    if (animator != nullptr) {
        animator.reset();
    }
}

const char *MovingView::name() {
    return "MovingView";
}

bool MovingView::forceRequestTouchMove() {
    return true;
}
