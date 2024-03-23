//
// Created by ByteDance on 2022/6/23.
//

#ifndef SKIAUI_IANIMATOR_H
#define SKIAUI_IANIMATOR_H

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

    virtual float getInterpolation(float factor) = 0;

    virtual void setDuration(long duration);

    virtual void start();

    virtual bool isEnd();

    virtual void update(SkIRect &rect) = 0;

    virtual void addListener(std::function<void()> finishCallback);

protected:

    long duration;

    long startTime, endTime;

    bool end;

    std::function<void()> finishCallback = nullptr;

};


#endif //SKIAUI_IANIMATOR_H