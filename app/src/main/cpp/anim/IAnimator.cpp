#include "IAnimator.h"

long IAnimator::currTime = 0L;

IAnimator::IAnimator() : duration(500L), end(true), startTime(-1L), endTime(-1) {
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

