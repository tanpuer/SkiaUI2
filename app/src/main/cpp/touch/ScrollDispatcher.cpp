#include "ScrollDispatcher.h"

ScrollDispatcher::ScrollDispatcher(ScrollView *view) : TouchEventDispatcher(view) {
    this->scrollView = view;
}

ScrollDispatcher::~ScrollDispatcher() = default;

bool ScrollDispatcher::onTouchEvent(TouchEvent *touchEvent) {
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            scrollView->stopFling();
            if (scrollView->_direction == YGFlexDirectionColumn) {
                startY = touchEvent->y;
            } else {
                startX = touchEvent->x;
            }
            break;
        }
        case TouchEvent::ACTION_MOVE: {
            ALOGD("ScrollDispatcher::ACTION_MOVE")
            if (scrollView->_direction == YGFlexDirectionColumn) {
                scrollView->updateTranslateY(touchEvent->y - startY);
                startY = touchEvent->y;
            } else {
                scrollView->updateTranslateX(touchEvent->x - startX);
                startX = touchEvent->x;
            }
            break;
        }
        case TouchEvent::ACTION_UP: {
            scrollView->startFling();
            break;
        }
        case TouchEvent::ACTION_CANCEL: {
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

void ScrollDispatcher::fling() {

}

View *ScrollDispatcher::findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent) {
    for (auto i = viewGroup->children.rbegin(); i != viewGroup->children.rend(); ++i) {
        auto child = *i;
        auto left = child->left;
        auto top = child->top;
        auto width = child->getWidth();
        auto height = child->getHeight();
        if (touchEvent->x >= left && touchEvent->x <= left + width &&
            touchEvent->y >= top && touchEvent->y <= top + height) {
            if (child->isViewGroup()) {
                return findTargetViewTraversal(dynamic_cast<ViewGroup *>(child), touchEvent);
            } else {
                return child;
            }
        }
    }
    ALOGD("findTargetViewTraversal null")
    return viewGroup;
}

bool ScrollDispatcher::onInterceptTouchEvent(TouchEvent *touchEvent) {
    if (weakTargetView == scrollView) {
        return true;
    }
    if (touchEvent->action == TouchEvent::ACTION_DOWN) {
        lastScrollX = 0.0f;
        lastScrollY = 0.0f;
    } else if (touchEvent->action == TouchEvent::ACTION_MOVE) {
        if (abs(touchEvent->x - lastScrollX) >= 5 || abs(touchEvent->y - lastScrollY) >= 5) {
            //TODO
            if (weakTargetView->forceRequestTouchMove()) {
                return false;
            }
            clearTargetView();
            weakTargetView = scrollView;
            startX = touchEvent->x;
            startY = touchEvent->y;
            return true;
        }
        lastScrollX = touchEvent->x;
        lastScrollY = touchEvent->y;
    }
    return TouchEventDispatcher::onInterceptTouchEvent(touchEvent);
}
