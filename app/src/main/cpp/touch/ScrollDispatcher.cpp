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

bool ScrollDispatcher::canScroll() {
    if (scrollView == nullptr) {
        return false;
    }
    return scrollView->canScroll();
}

void ScrollDispatcher::findTargetView(TouchEvent *touchEvent) {
    //todo
    TouchEventDispatcher::findTargetView(touchEvent);
}

void ScrollDispatcher::dispatchToTargetView(TouchEvent *touchEvent) {
    if (weakTargetView != nullptr) {
        auto consumed = weakTargetView->onTouchEvent(touchEvent);
        if (!consumed && touchEvent->action == TouchEvent::ACTION_UP) {
            weakTargetView->performClick();
        }
        if (consumed) {
            //如果已经被消费了，ScrollView就不能消费去滑动了
            return;
        }
    }
    if (canScroll()) {
        scrollView->onTouchEvent(touchEvent);
    }
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
                targetViewLeft = left;
                targetViewTop = top;
                return child;
            }
        }
    }
    ALOGD("findTargetViewTraversal null")
    return viewGroup;
}
