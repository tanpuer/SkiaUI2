#pragma once

#include "functional"
#include "core/SkRect.h"

class IAnimator {

public:

    /**
     * 每画一帧，会记录当前的时间戳，其他动画或者变化可参考此时间
     */
    static long currTime;

public:

    IAnimator();

    virtual ~IAnimator();

    virtual float getInterpolation(float factor);

    virtual void setDuration(long duration);

    virtual void start();

    virtual bool isEnd();

    virtual void update(SkIRect &rect) = 0;

    virtual void addListener(std::function<void()> finishCallback);

    virtual void setLoopCount(int count);

protected:

    long duration;

    long startTime, endTime;

    bool end;

    int loopCount = 1;

    std::function<void()> finishCallback = nullptr;

};
