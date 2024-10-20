#pragma once

#include "TouchEvent.h"
#include "memory"
#include "Velocity.h"

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

    virtual void clearTargetView(TouchEvent *touchEvent);

    virtual View *findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent);

    bool checkTouchInTargetView(TouchEvent *touchEvent);

    virtual bool dispatchVelocity(Velocity *velocity);

protected:

    View *view;

    //todo 改用weak_ptr
    View *weakTargetView;

    float lastAlpha = 1.0f;

};
