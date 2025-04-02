#include "LinearAnimator.h"

namespace HYSkiaUI {

LinearAnimator::LinearAnimator(View *view, float startValue, float endValue)
        : startValue(startValue), endValue(endValue) {
    this->targetView = view;
    view->setAnimator(this);
}

void LinearAnimator::update(SkIRect &rect) {
    if (paused) {
        return;
    }
    if (currTime > endTime) {
        currLoopCount++;
        if (loopCount == -1 || currLoopCount < loopCount) {
            startTime = currTime;
            endTime = currTime + duration;
            if (autoReverse) {
                auto temp = startValue;
                startValue = endValue;
                endValue = temp;
            }
        } else {
            end = true;
            updateInner();
            if (!jsUpdateCallback.IsEmpty()) {
                jsUpdateCallback.Reset();
            }
            return;
        }
    }
    updateInner();
}

void LinearAnimator::setUpdateListener(std::function<void(View *, float)> &&listener) {
    this->updateListener = std::move(listener);
}

void LinearAnimator::updateInner() {
    if (targetView != nullptr && updateListener != nullptr) {
        auto value = sEaseLst[easeType](static_cast<float >(currTime - startTime), startValue, endValue - startValue, duration);
        updateListener(targetView, value);
        if (!paused) {
            targetView->markDirty();
        }
    }
}

}
