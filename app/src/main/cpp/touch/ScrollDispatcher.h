#pragma once

#include <ScrollView.h>
#include "TouchEventDispatcher.h"

class ScrollDispatcher : public TouchEventDispatcher {

public:

    friend class ScrollView;

    ScrollDispatcher(ScrollView *view);

    virtual ~ScrollDispatcher();

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    virtual View *findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent) override;

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

protected:

    /**
     * 当手离开屏幕，scrollView还能滑动时，需要使用动画模拟fling的效果
     */
    virtual void fling();

    float startX = 0.0f;
    float startY = 0.0f;

    ScrollView *scrollView;

    float lastScrollX = 0.0f;
    float lastScrollY = 0.0f;

};
