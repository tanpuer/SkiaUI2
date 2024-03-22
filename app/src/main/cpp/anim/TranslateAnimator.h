//
// Created by banma-3412 on 2024/3/22.
//

#ifndef SKIAUI_TRANSLATEANIMATOR_H
#define SKIAUI_TRANSLATEANIMATOR_H


#include "IAnimator.h"
#include "View.h"

class TranslateAnimator : public IAnimator {

public:

    TranslateAnimator(View *view, float translateX, float translateY);

    virtual ~TranslateAnimator();

    virtual void update(SkIRect &rect) override;

    virtual void start() override;

    float getInterpolation(float factor) override;

private:

    float animTransX, animTransY;

    View *targetView = nullptr;

};


#endif //SKIAUI_TRANSLATEANIMATOR_H
