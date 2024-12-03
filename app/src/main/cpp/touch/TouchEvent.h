#pragma once

#include "native_log.h"

namespace HYSkiaUI {

class TouchEvent {

public:

    enum MotionEvent {
        ACTION_DOWN = 0,
        ACTION_UP = 1,
        ACTION_MOVE = 2,
        ACTION_CANCEL = 3
    };

    TouchEvent(MotionEvent action, float x, float y) : action(action), x(x), y(y) {

    }

    ~TouchEvent() {
        ALOGD("TouchEvent %s", "~TouchEvent")
    }

public:
    MotionEvent action;
    float x, y;

};

}
