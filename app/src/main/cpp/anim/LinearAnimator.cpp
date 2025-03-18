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
        if (loopCount == -1) {
            startTime = currTime;
            endTime = currTime + duration;
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
        auto interpolator = end ? 1.0f : getInterpolation(1.0f);
        auto value = interpolator * (endValue - startValue) + startValue;
        updateListener(targetView, value);
        if (!paused) {
            targetView->markDirty();
        }
    }
}

}
