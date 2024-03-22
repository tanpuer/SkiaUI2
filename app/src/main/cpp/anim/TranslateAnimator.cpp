//
// Created by banma-3412 on 2024/3/22.
//

#include "TranslateAnimator.h"

TranslateAnimator::TranslateAnimator(View *view, float translateX, float translateY) {
    this->targetView = view;
    this->animTransX = translateX;
    this->animTransY = translateY;
}

TranslateAnimator::~TranslateAnimator() {
    this->targetView = nullptr;
}

void TranslateAnimator::update(SkIRect &rect) {
    if (targetView == nullptr) {
        return;
    }
    auto interpolator = getInterpolation(1.0f);
    auto targetLeft = animTransX * interpolator + targetView->left;
    auto targetTop = animTransY * interpolator + targetView->top;
}

void TranslateAnimator::start() {
    IAnimator::start();
}

float TranslateAnimator::getInterpolation(float factor) {
    return static_cast<float >(endTime - currTime) / static_cast<float >(duration);
}
