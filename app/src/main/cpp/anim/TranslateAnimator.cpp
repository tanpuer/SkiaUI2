#include "TranslateAnimator.h"

namespace HYSkiaUI {

TranslateAnimator::TranslateAnimator(View *view,
                                     float animStartX, float animEndX,
                                     float animStartY, float animEndY) {
    view->setAnimator(this);
    this->targetView = view;
    view->animTranslateX = animStartX;
    view->animTranslateY = animStartY;
    this->animEndX = animEndX;
    this->animEndY = animEndY;
}

TranslateAnimator::~TranslateAnimator() {
    if (finishCallback != nullptr) {
        finishCallback();
    }
    this->targetView = nullptr;
}

void TranslateAnimator::update(SkRect &rect) {
    if (currTime > endTime || targetView == nullptr) {
        targetView->animTranslateX = animEndX;
        targetView->animTranslateY = animEndY;
        end = true;
        return;
    }
    auto interpolator = getInterpolation(1.0f);
    targetView->animTranslateX = animStartX + (animEndX - animStartX) * interpolator;
    targetView->animTranslateY = animStartY + (animEndY - animStartY) * interpolator;
    targetView->markDirty();
}

void TranslateAnimator::start() {
    IAnimator::start();
    SkASSERT(targetView);
    animStartX = targetView->animTranslateX;
    animStartY = targetView->animTranslateY;
}

}
