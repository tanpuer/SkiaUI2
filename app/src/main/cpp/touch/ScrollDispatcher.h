//
// Created by ByteDance on 2022/7/26.
//

#ifndef SKIAUI_SCROLLDISPATCHER_H
#define SKIAUI_SCROLLDISPATCHER_H

#include <ScrollView.h>
#include "TouchEventDispatcher.h"

class ScrollDispatcher : public TouchEventDispatcher {

public:

    friend class ScrollView;

    ScrollDispatcher(ScrollView *view);

    virtual ~ScrollDispatcher();

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    virtual void findTargetView(TouchEvent *touchEvent) override;

    virtual void dispatchToTargetView(TouchEvent *touchEvent) override;

    virtual View *
    findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent, float tempLeft,
                            float tempTop) override;

protected:

    virtual bool canScroll();

    /**
     * 当手离开屏幕，scrollView还能滑动时，需要使用动画模拟fling的效果
     */
    virtual void fling();

    float startX = 0.0f;
    float startY = 0.0f;

    ScrollView *scrollView;

};


#endif //SKIAUI_SCROLLDISPATCHER_H
