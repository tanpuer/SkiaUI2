#pragma once

#include "functional"
#include "core/SkRect.h"

namespace HYSkiaUI {

static int32_t ANIMATOR_ID = 0;

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

    virtual void addListener(std::function<void()>&& finishCallback);

    virtual void setLoopCount(int count);

    virtual void pause();

    virtual void resume();

    virtual void stop();

    virtual uint32_t getAnimatorId();

protected:

    long duration;

    long startTime, endTime;

    bool end;

    int loopCount = 1;

    std::function<void()> finishCallback = nullptr;

    long pausedTime = 0L;

    bool paused = false;

    uint32_t animatorId = 0;

    float lastInterpolator = 0.0f;

};

}
