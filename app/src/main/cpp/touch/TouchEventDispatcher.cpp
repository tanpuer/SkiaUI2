#include "TouchEventDispatcher.h"
#include <yoga/Yoga.h>
#include "ViewGroup.h"
#include "MovingView.h"
#include "ScrollView.h"

namespace HYSkiaUI {

TouchEventDispatcher::TouchEventDispatcher(View *view) {
    this->view = view;
    weakTargetView = nullptr;
}

TouchEventDispatcher::~TouchEventDispatcher() = default;

bool TouchEventDispatcher::dispatchTouchEvent(TouchEvent *touchEvent) {
    ALOGD("dispatchTouchEvent type is %d", touchEvent->action)
    if (view == nullptr) {
        ALOGE("dispatchTouchEvent weakRefView is null, pls check")
        return false;
    }
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            findTargetView(touchEvent);
            if (weakTargetView != nullptr) {
                weakTargetView->onInterceptTouchEvent(touchEvent);
            }
            dispatchToTargetView(touchEvent);
            break;
        }
        case TouchEvent::ACTION_MOVE: {
            if (!checkTouchInTargetView(touchEvent)) {
                clearTargetView(touchEvent);
            }
            if (weakTargetView != nullptr) {
                if (weakTargetView->onInterceptTouchEvent(touchEvent)) {
                    dispatchToTargetView(touchEvent);
                } else {
                    auto parent = weakTargetView->getParent();
                    while (parent != nullptr) {
                        if (parent->onInterceptTouchEvent(touchEvent)) {
                            clearTargetView(touchEvent);
                            weakTargetView = parent;
                            touchEvent->action = TouchEvent::ACTION_DOWN;
                            dispatchToTargetView(touchEvent);
                            touchEvent->action = TouchEvent::ACTION_MOVE;
                            dispatchToTargetView(touchEvent);
                            break;
                        } else {
                            parent = parent->getParent();
                        }
                    }
                }
            }
            break;
        }
        case TouchEvent::ACTION_UP: {
            dispatchToTargetView(touchEvent);
            clearTargetView(nullptr);
            break;
        }
        case TouchEvent::ACTION_CANCEL: {
            dispatchToTargetView(touchEvent);
            clearTargetView(nullptr);
            break;
        }
        default: {
            break;
        }
    }
    view->markDirty();
    return true;
}

bool TouchEventDispatcher::onInterceptTouchEvent(TouchEvent *touchEvent) {
    return false;
}

bool TouchEventDispatcher::onTouchEvent(TouchEvent *touchEvent) {
    if (view == nullptr) {
        ALOGE("dispatchTouchEvent weakRefView is null, pls check")
        return false;
    }
    return false;
}

void TouchEventDispatcher::requestDisallowInterceptTouchEvent(bool disallowIntercept) {

}

void TouchEventDispatcher::setWeakView(View *view) {

}

void TouchEventDispatcher::findTargetView(TouchEvent *touchEvent) {
    if (view == nullptr) {
        ALOGE("dispatchTouchEvent weakRefView is null, pls check")
        return;
    }
    if (weakTargetView != nullptr) {
        ALOGE("dispatchTouchEvent error: weakRefView is not null")
        clearTargetView(touchEvent);
    }
    auto viewGroup = dynamic_cast<ViewGroup *>(view);
    if (viewGroup == nullptr) {
        ALOGE("dispatchTouchEvent weakRefView is not ViewGroup, pls check")
        return;
    }
    weakTargetView = findTargetViewTraversal(viewGroup, touchEvent);
    if (weakTargetView != nullptr) {
        lastAlpha = weakTargetView->getAlpha();
        weakTargetView->setAlpha(0.3f);
    }
}

void TouchEventDispatcher::dispatchToTargetView(TouchEvent *touchEvent) {
    if (touchEvent->action == TouchEvent::ACTION_MOVE) {
        ALOGD("ScrollDispatcher::ACTION_MOVE %d", weakTargetView != nullptr);
    }
    if (weakTargetView != nullptr) {
        auto consumed = weakTargetView->onTouchEvent(touchEvent);
        if (!consumed && touchEvent->action == TouchEvent::ACTION_UP) {
            weakTargetView->performClick();
        }
    }
}

void TouchEventDispatcher::clearTargetView(TouchEvent *touchEvent) {
    if (weakTargetView != nullptr) {
        weakTargetView->setAlpha(lastAlpha);
        if (touchEvent != nullptr) {
            auto originAction = touchEvent->action;
            touchEvent->action = TouchEvent::ACTION_CANCEL;
            weakTargetView->onTouchEvent(touchEvent);
            touchEvent->action = originAction;
        }
        ALOGD("dispatchTouchEvent, clearTargetView success")
        weakTargetView = nullptr;
        lastScrollX = 0.0f;
        lastScrollY = 0.0f;
    }
}

View *TouchEventDispatcher::findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent) {
    ALOGD("findTargetViewTraversal %s %ld", viewGroup->name(), viewGroup->children.size())
    for (auto i = viewGroup->children.rbegin(); i != viewGroup->children.rend(); ++i) {
        auto child = *i;
        auto left = child->getLeft();
        auto top = child->getTop();
        auto width = child->getWidth();
        auto height = child->getHeight();
        ALOGD("findTargetViewTraversal %d %d %d %d %f %f", left, top, width, height, touchEvent->x,
              touchEvent->y)
        if (touchEvent->x >= left && touchEvent->x <= left + width &&
            touchEvent->y >= top && touchEvent->y <= top + height) {
            if (child->isViewGroup() && !child->isScroller()) {
                ALOGD("findTargetViewTraversal in ViewGroup %s %ld", child->name(), child->getViewId())
                return findTargetViewTraversal(dynamic_cast<ViewGroup *>(child), touchEvent);
            } else {
                ALOGD("findTargetViewTraversal result %s %ld", child->name(), child->getViewId())
                if (child->isScroller()) {
                    return findTargetViewTraversal(dynamic_cast<ScrollView*>(child), touchEvent);
                }
                return child;
            }
        }
    }
    ALOGD("findTargetViewTraversal null")
    return viewGroup;
}

bool TouchEventDispatcher::checkTouchInTargetView(TouchEvent *touchEvent) {
    if (weakTargetView == nullptr || weakTargetView->getNode() == nullptr) {
        return false;
    }
    if (weakTargetView->onInterceptTouchEvent(touchEvent)) {
        return true;
    }
    auto left = weakTargetView->getLeft();
    auto top = weakTargetView->getTop();
    auto right = weakTargetView->getRight();
    auto bottom = weakTargetView->getBottom();
    return touchEvent->x >= left && touchEvent->x <= right
           && touchEvent->y >= top && touchEvent->y <= bottom;
}

bool TouchEventDispatcher::dispatchVelocity(Velocity *velocity) {
    if (weakTargetView != nullptr) {
        weakTargetView->setVelocity(velocity->xVelocity, velocity->yVelocity);
        return true;
    }
    return false;
}

}
