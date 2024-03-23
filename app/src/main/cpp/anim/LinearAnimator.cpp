//
// Created by ByteDance on 2022/6/23.
//

#include "LinearAnimator.h"
#include "native_log.h"
#include "core/SkMatrix.h"

LinearAnimator::LinearAnimator(float translateX, float translateY)
        : IAnimator(), translateX(translateX), translateY(translateY) {
    dst = SkRect::MakeEmpty();
    src = SkRect::MakeEmpty();
    translateMatrix = SkMatrix::I();
    rotateMatrix = SkMatrix::I();
    scaleMatrix = SkMatrix::I();
    m = SkMatrix::I();
}

LinearAnimator::~LinearAnimator() {

}

float LinearAnimator::getInterpolation(float factor) {
    return static_cast<float >(endTime - currTime) / static_cast<float >(duration);
}

void LinearAnimator::update(SkIRect &rect, AnimationResult &animationResult) {
    if (currTime > endTime) {
        end = true;
        return;
    }
    auto interpolation = getInterpolation(1.0f);
    ALOGD("LinearAnimator::update %ld %ld %f", currTime, endTime, interpolation)
    auto x = translateX * interpolation;
    auto y = translateY * interpolation;

    src.set(rect);
    m.setIdentity();
    translateMatrix.setIdentity();
    rotateMatrix.setIdentity();
    scaleMatrix.setIdentity();
    translateMatrix.setTranslate(x, y);
    //todo rotate设置不对
//    rotateMatrix.setRotate(45, src.centerX(), src.centerY());
//    scaleMatrix.setScale(1 - interpolation, 1 - interpolation, src.centerX(), src.centerY());
    m.preConcat(translateMatrix);
    m.preConcat(rotateMatrix);
    m.preConcat(scaleMatrix);
    m.mapRect(&dst, src, SkApplyPerspectiveClip::kNo);
    rect.setLTRB(dst.left(), dst.top(), dst.right(), dst.bottom());
}
