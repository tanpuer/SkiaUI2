//
// Created by banma-3412 on 2024/3/22.
//

#include "TranslateAnimator.h"

TranslateAnimator::TranslateAnimator(View *view, float animEndX, float animEndY) {
    this->targetView = view;
    this->animEndX = animEndX;
    this->animEndY = animEndY;
}

TranslateAnimator::~TranslateAnimator() {
    this->targetView = nullptr;
}

void TranslateAnimator::update(SkIRect &rect, AnimationResult &animationResult) {
    if (currTime > endTime || targetView == nullptr) {
        end = true;
        return;
    }
    auto interpolator = getInterpolation(1.0f);
    animationResult.translateX = animStartX + (animEndX - animStartX) * interpolator;
    animationResult.translateY = animStartY + (animEndY - animStartY) * interpolator;
}

void TranslateAnimator::start() {
    IAnimator::start();
    SkASSERT(targetView);
    animStartX = targetView->animTranslateX;
    animStartY = targetView->animTranslateY;
}

float TranslateAnimator::getInterpolation(float factor) {
    return static_cast<float >(endTime - currTime) / static_cast<float >(duration);
}
