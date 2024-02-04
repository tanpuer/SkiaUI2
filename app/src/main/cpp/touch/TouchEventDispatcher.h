//
// Created by cw on 2022/6/4.
//

#ifndef SKIAUI_TOUCHEVENTDISPATCHER_H
#define SKIAUI_TOUCHEVENTDISPATCHER_H

#include "TouchEvent.h"
#include "memory"

class View;

class ViewGroup;

class TouchEventDispatcher {

public:

    TouchEventDispatcher(View *view);

    ~TouchEventDispatcher();

    virtual bool dispatchTouchEvent(TouchEvent *touchEvent);

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent);

    virtual bool onTouchEvent(TouchEvent *touchEvent);

    virtual void requestDisallowInterceptTouchEvent(bool disallowIntercept);

    virtual void setWeakView(View *view);

    virtual void findTargetView(TouchEvent *touchEvent);

    virtual void dispatchToTargetView(TouchEvent *touchEvent);

    virtual void clearTargetView();

    virtual View *
    findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent, float tempLeft,
                            float tempTop);

    bool checkTouchInTargetView(TouchEvent *touchEvent);

protected:

    View *view;

    //todo 改用weak_ptr
    View *weakTargetView;

    float targetViewLeft = 0.0f;
    float targetViewTop = 0.0f;

};


#endif //SKIAUI_TOUCHEVENTDISPATCHER_H
