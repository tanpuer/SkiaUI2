#pragma once

#include "IAnimator.h"
#include "View.h"

namespace HYSkiaUI {

class TranslateAnimator : public IAnimator {

public:

    TranslateAnimator(View *view,
                      float animStartX, float animEndX,
                      float animStartY, float animEndY);

    virtual ~TranslateAnimator();

    virtual void update(SkRect &rect) override;

    virtual void start() override;

private:

    float animEndX, animEndY = 0.0f;

    float animStartX, animStartY = 0.0f;

    View *targetView = nullptr;

};

}
