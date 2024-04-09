#pragma once

#include "TouchEvent.h"
#include "memory"
#include "ITestDraw.h"
#include "Velocity.h"

/**
 * UI线程执行的逻辑入口
 */
class SkiaUIApp {

public:

    SkiaUIApp();

    ~SkiaUIApp();

    void setWindowSize(int width, int height);

    void dispatchTouchEvent(TouchEvent *touchEvent);

    void setVelocity(Velocity *velocity);

    long doFrame(long time);

    bool onBackPressed();

private:

    std::unique_ptr<TouchEvent> mTouchEvent;

    std::unique_ptr<ITestDraw> testDraw;

    int mWidth = 0, mHeight = 0;

    int drawCount = 0;

};