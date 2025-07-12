#pragma once

#include <ScrollView.h>
#include "TouchEventDispatcher.h"

namespace HYSkiaUI {

class ScrollDispatcher : public TouchEventDispatcher {

public:

    friend class ScrollView;

    ScrollDispatcher(ScrollView *view);

    virtual ~ScrollDispatcher();

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    virtual View *findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent) override;

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

protected:

    ScrollView *scrollView;

    void resetLastScroll();

};

}