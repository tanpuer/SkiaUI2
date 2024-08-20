#include "RotateAnimator.h"

RotateAnimator::RotateAnimator(View *view, float rotateStart, float rotateEnd)
        : rotateStart(rotateStart), rotateEnd(rotateEnd) {
    this->targetView = view;
    view->setAnimator(this);
}

void RotateAnimator::update(SkIRect &rect) {
    if (currTime > endTime) {
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

void RotateAnimator::setUpdateListener(std::function<void(View *, float)> &&listener) {
    this->updateListener = std::move(listener);
}

void RotateAnimator::updateInner() {
    if (targetView != nullptr && updateListener != nullptr) {
        auto interpolator = end ? 1.0f : getInterpolation(1.0f);
        auto value = interpolator * (rotateEnd - rotateStart) + rotateStart;
        updateListener(targetView, value);
    }
}
