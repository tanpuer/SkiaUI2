#pragma once

#include "IAnimator.h"
#include "View.h"

namespace HYSkiaUI {

class LinearAnimator : public IAnimator {

public:

    LinearAnimator(View *view, float startValue, float endValue);

    void update(SkRect &rect) override;

    void setUpdateListener(std::function<void(View *, float)> &&listener);

public:

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> jsUpdateCallback;

private:

    float startValue = 0.0f;

    float endValue = 0.0f;

    View *targetView = nullptr;

    std::function<void(View *, float)> updateListener = nullptr;

    void updateInner();

};

}
