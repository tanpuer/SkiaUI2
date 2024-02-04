//
// Created by cw on 1/21/22.
//

#ifndef SKIAUI_IFILTER_H
#define SKIAUI_IFILTER_H


#include <TouchEvent.h>
#include "touch/Velocity.h"

class IFilter {

public:

    IFilter() {};

    virtual ~IFilter() {};

    virtual void setWindowSize(int width, int height) = 0;

    virtual void doFrame(long time) {
        this->time = time;
        drawCount++;
        if (drawCount > width) {
            drawCount = 0;
        }
    };

    virtual void dispatchTouchEvent(TouchEvent *touchEvent) {}

    virtual void setVelocity(Velocity *velocity) {}

protected:

    int width = 0;
    int height = 0;

    int drawCount = 0;

    long time;

};


#endif //SKIAUI_IFILTER_H
