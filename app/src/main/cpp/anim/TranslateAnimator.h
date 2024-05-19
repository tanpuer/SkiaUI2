#pragma once

#include "IAnimator.h"
#include "View.h"

class TranslateAnimator : public IAnimator {

public:

    TranslateAnimator(View *view,
                      float animStartX, float animEndX,
                      float animStartY, float animEndY);

    virtual ~TranslateAnimator();

    virtual void update(SkIRect &rect) override;

    virtual void start() override;

    float getInterpolation(float factor) override;

private:

    float animEndX, animEndY = 0.0f;

    float animStartX, animStartY = 0.0f;

    View *targetView = nullptr;

};
