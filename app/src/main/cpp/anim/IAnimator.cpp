#include "IAnimator.h"

long IAnimator::currTime = 0L;

IAnimator::IAnimator() : duration(500L), end(true), startTime(-1L), endTime(-1), loopCount(1) {
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

void IAnimator::addListener(std::function<void()> finishCallback) {
    this->finishCallback = std::move(finishCallback);
}

void IAnimator::setLoopCount(int count) {
    this->loopCount = count;
}

float IAnimator::getInterpolation(float factor) {
    if (paused) {
        return static_cast<float >(pausedStartTime - pausedTotalTime) / static_cast<float >(duration);
    } else {
        return static_cast<float >(currTime - pausedTotalTime - startTime) / static_cast<float >(duration);
    }
}

void IAnimator::pause() {
    if (paused) {
        return;
    }
    paused = true;
    pausedStartTime = currTime;
}

void IAnimator::resume() {
    if (!paused) {
        return;
    }
    paused = false;
    pausedTotalTime += currTime - pausedStartTime;
}

