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
        lastScrollX = touchEvent->x;
        lastScrollY = touchEvent->y;
    } else if (touchEvent->action == TouchEvent::ACTION_MOVE) {
        auto diffX = abs(touchEvent->x - lastScrollX);
        auto diffY = abs(touchEvent->y - lastScrollY);
        if ((scrollView->_direction == YGFlexDirectionColumn && diffY > diffX &&
             diffY > ScrollView::SCROLL_SLOP) ||
            (scrollView->_direction == YGFlexDirectionRow && diffX > diffY &&
             diffX > ScrollView::SCROLL_SLOP)) {
            //TODO
            if (weakTargetView != nullptr && weakTargetView->forceRequestTouchMove()) {
                return false;
            }
            clearTargetView(touchEvent);
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
