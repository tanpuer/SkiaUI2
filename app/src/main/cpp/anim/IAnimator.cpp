#include "IAnimator.h"

namespace HYSkiaUI {

long IAnimator::currTime = 0L;

IAnimator::IAnimator() : duration(500L), end(true), startTime(-1L), endTime(-1), loopCount(1) {
    animatorId = ANIMATOR_ID++;
}

IAnimator::~IAnimator() {

}

void IAnimator::setDuration(long duration) {
    this->duration = duration;
}

void IAnimator::start() {
    end = false;
    startTime = currTime;
    endTime = currTime + duration;
}

bool IAnimator::isEnd() {
    return end;
}

void IAnimator::addListener(std::function<void()> &&finishCallback) {
    this->finishCallback = std::move(finishCallback);
}

void IAnimator::setLoopCount(int count) {
    this->loopCount = count;
}

float IAnimator::getInterpolation(float factor) {
    if (!paused) {
        lastInterpolator =
                static_cast<float >(currTime - startTime) / static_cast<float >(duration);
    }
    return lastInterpolator;
}

void IAnimator::pause() {
    if (paused) {
        return;
    }
    paused = true;
    pausedTime = currTime;
}

void IAnimator::resume() {
    if (!paused) {
        return;
    }
    paused = false;
    startTime += currTime - pausedTime;
    endTime += currTime - pausedTime;
}

uint32_t IAnimator::getAnimatorId() {
    return animatorId;
}

void IAnimator::stop() {
    end = true;
}

void IAnimator::setEaseType(EaseType type) {
    this->easeType = static_cast<uint8_t>(type);
}

}
