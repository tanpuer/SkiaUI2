//
// Created by banma-3412 on 2024/3/22.
//

#ifndef SKIAUI_TRANSLATEANIMATOR_H
#define SKIAUI_TRANSLATEANIMATOR_H


#include "IAnimator.h"
#include "View.h"

class TranslateAnimator : public IAnimator {

public:

    TranslateAnimator(View *view, float animEndX, float animEndY);

    virtual ~TranslateAnimator();

    virtual void update(SkIRect &rect, AnimationResult &animationResult) override;

    virtual void start() override;

    float getInterpolation(float factor) override;

private:

    float animEndX, animEndY = 0.0f;

    float animStartX, animStartY = 0.0f;

    View *targetView = nullptr;

};


#endif //SKIAUI_TRANSLATEANIMATOR_H
