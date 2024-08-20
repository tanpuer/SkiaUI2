#pragma once

#include "IAnimator.h"
#include "core/SkMatrix.h"

class LinearAnimator : public IAnimator {

public:

    enum class AnimatorType {
        Scale,
        Rotate,
        Translate,
    };

public:

    LinearAnimator(float translateX, float translateY);

    virtual ~LinearAnimator();

    void update(SkIRect &rect) override;

private:

    float translateX, translateY;

    float startRotate, endRotate;

    SkRect dst, src;

    SkMatrix m, translateMatrix, rotateMatrix, scaleMatrix;

};
