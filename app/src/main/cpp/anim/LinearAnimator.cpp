#include "LinearAnimator.h"

namespace HYSkiaUI {

LinearAnimator::LinearAnimator(View *view, float startValue, float endValue)
        : startValue(startValue), endValue(endValue) {
    this->targetView = view;
    view->setAnimator(this);
}

void LinearAnimator::update(SkIRect &rect) {
    if (currTime > endTime && !paused) {
        if (loopCount == -1) {
            startTime = currTime;
            endTime = currTime + duration;
        } else {
            end = true;
            updateInner();
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
        targetView->markDirty();
    }
}

}
