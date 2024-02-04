//
// Created by cw on 2022/6/4.
//

#include "TouchEventDispatcher.h"
#include <yoga/Yoga.h>
#include "ViewGroup.h"
#include "MovingView.h"

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
    if (onInterceptTouchEvent(touchEvent)) {
        return false;
    }
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            findTargetView(touchEvent);
            dispatchToTargetView(touchEvent);
            break;
        }
        case TouchEvent::ACTION_MOVE: {
            if (!checkTouchInTargetView(touchEvent)) {
                clearTargetView();
            }
            dispatchToTargetView(touchEvent);
            break;
        }
        case TouchEvent::ACTION_UP: {
            dispatchToTargetView(touchEvent);
            clearTargetView();
            break;
        }
        case TouchEvent::ACTION_CANCEL: {
            dispatchToTargetView(touchEvent);
            clearTargetView();
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

bool TouchEventDispatcher::onInterceptTouchEvent(TouchEvent *touchEvent) {
    if (view == nullptr) {
        ALOGE("dispatchTouchEvent weakRefView is null, pls check")
        return false;
    }
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
        clearTargetView();
    }
    auto viewGroup = dynamic_cast<ViewGroup *>(view);
    if (viewGroup == nullptr) {
        ALOGE("dispatchTouchEvent weakRefView is not ViewGroup, pls check")
        return;
    }
    weakTargetView = findTargetViewTraversal(viewGroup, touchEvent, 0.0, 0.0);
    if (weakTargetView != nullptr) {
        weakTargetView->setAlpha(0.3f);
    }
}

void TouchEventDispatcher::dispatchToTargetView(TouchEvent *touchEvent) {
    if (weakTargetView != nullptr) {
        auto consumed = weakTargetView->onTouchEvent(touchEvent);
        if (!consumed && touchEvent->action == TouchEvent::ACTION_UP) {
            weakTargetView->performClick();
        }
    }
}

void TouchEventDispatcher::clearTargetView() {
    if (weakTargetView != nullptr) {
        weakTargetView->setAlpha(1.0f);
        ALOGD("dispatchTouchEvent, clearTargetView success")
        weakTargetView = nullptr;
    }
}

View *TouchEventDispatcher::findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent,
                                                    float tempLeft, float tempTop) {
    ALOGD("findTargetViewTraversal %s %d", viewGroup->name(), viewGroup->children.size())
    for (auto i = viewGroup->children.rbegin(); i != viewGroup->children.rend(); ++i) {
        auto child = *i;
        auto left = YGNodeLayoutGetLeft(child->node) + tempLeft;
        auto top = YGNodeLayoutGetTop(child->node) + tempTop;
        auto width = YGNodeLayoutGetWidth(child->node);
        auto height = YGNodeLayoutGetHeight(child->node);
        ALOGD("findTargetViewTraversal %f %f %f %f %f %f", left, top, width, height, touchEvent->x,
              touchEvent->y)
        if (touchEvent->x >= left && touchEvent->x <= left + width &&
            touchEvent->y >= top && touchEvent->y <= top + height) {
            if (child->isViewGroup()) {
                ALOGD("findTargetViewTraversal in ViewGroup %s %lld", child->name(), child->viewId)
                return findTargetViewTraversal(dynamic_cast<ViewGroup *>(child), touchEvent, left,
                                               top);
            } else {
                ALOGD("findTargetViewTraversal result %s %lld", child->name(), child->viewId)
                targetViewLeft = left;
                targetViewTop = top;
                return child;
            }
        }
    }
    ALOGD("findTargetViewTraversal null")
    return viewGroup;
}

bool TouchEventDispatcher::checkTouchInTargetView(TouchEvent *touchEvent) {
    if (weakTargetView == nullptr || weakTargetView->node == nullptr) {
        return false;
    }
    if (weakTargetView->forceRequestTouchMove()) {
        return true;
    }
    auto width = YGNodeLayoutGetWidth(weakTargetView->node);
    auto height = YGNodeLayoutGetHeight(weakTargetView->node);
    return touchEvent->x >= targetViewLeft && touchEvent->x <= targetViewLeft + width
           && touchEvent->y >= targetViewTop && touchEvent->y <= targetViewTop + height;
}
