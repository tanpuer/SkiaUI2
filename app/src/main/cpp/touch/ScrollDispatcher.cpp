#include "ScrollDispatcher.h"
#include "View.h"
#include "w3c_util.h"

namespace HYSkiaUI {

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
                scrollView->setScrollEnd(touchEvent->y < startY);
                scrollView->updateTranslateY(touchEvent->y - startY);
                startY = touchEvent->y;
            } else {
                scrollView->setScrollEnd(touchEvent->x < startX);
                scrollView->updateTranslateX(touchEvent->x - startX);
                startX = touchEvent->x;
            }
            break;
        }
        case TouchEvent::ACTION_UP: {
            scrollView->startFling();
            resetLastScroll();
            weakTargetView = nullptr;
            break;
        }
        case TouchEvent::ACTION_CANCEL: {
            resetLastScroll();
            weakTargetView = nullptr;
            break;
        }
        default: {
            break;
        }
    }
    scrollView->markDirty();
    return true;
}

void ScrollDispatcher::fling() {

}

View *ScrollDispatcher::findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent) {
    for (auto i = viewGroup->children.rbegin(); i != viewGroup->children.rend(); ++i) {
        auto child = *i;
        if (child->isTouchInRect(touchEvent->x, touchEvent->y)) {
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
    if (touchEvent->action == TouchEvent::ACTION_DOWN) {
        lastScrollX = touchEvent->x;
        lastScrollY = touchEvent->y;
        if (weakTargetView == scrollView) {
            return true;
        }
    } else if (touchEvent->action == TouchEvent::ACTION_MOVE) {
        if (YGFloatsEqual(0.0f, lastScrollX) && YGFloatsEqual(0.0f, lastScrollY)) {
            lastScrollX = touchEvent->x;
            lastScrollY = touchEvent->y;
            return false;
        }
        auto diffX = abs(touchEvent->x - lastScrollX);
        auto diffY = abs(touchEvent->y - lastScrollY);
        if ((scrollView->_direction == YGFlexDirectionColumn && diffY > diffX &&
             diffY > ScrollView::SCROLL_SLOP) ||
            (scrollView->_direction == YGFlexDirectionRow && diffX > diffY &&
             diffX > ScrollView::SCROLL_SLOP)) {
            if (weakTargetView == scrollView) {
                return true;
            }
            //TODO
            if (weakTargetView != nullptr && weakTargetView->onInterceptTouchEvent(touchEvent)) {
                return false;
            }
            clearTargetView(touchEvent);
            weakTargetView = scrollView;
            lastAlpha = weakTargetView->getAlpha();
            startX = touchEvent->x;
            startY = touchEvent->y;
            return true;
        }
        lastScrollX = touchEvent->x;
        lastScrollY = touchEvent->y;
    } else if (touchEvent->action == TouchEvent::ACTION_CANCEL || touchEvent->action == TouchEvent::ACTION_UP) {
        scrollView->startFling();
    }
    return TouchEventDispatcher::onInterceptTouchEvent(touchEvent);
}

void ScrollDispatcher::resetLastScroll() {
    lastScrollX = 0.0f;
    lastScrollY = 0.0f;
}

}
