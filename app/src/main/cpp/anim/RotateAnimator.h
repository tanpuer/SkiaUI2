#pragma once

#include "IAnimator.h"
#include "View.h"

class RotateAnimator : public IAnimator {

public:

    RotateAnimator(View *view, float rotateStart, float rotateEnd);

    void update(SkIRect &rect) override;

    void setUpdateListener(std::function<void(View *, float)> &&listener);

private:

    float rotateStart, rotateEnd;
    View *targetView;
    std::function<void(View *, float)> updateListener = nullptr;

    void updateInner();

};
