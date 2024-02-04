//
// Created by ByteDance on 2022/6/22.
//

#ifndef SKIAUI_MOVINGDRAWTEST_H
#define SKIAUI_MOVINGDRAWTEST_H

#include "ITestDraw.h"

/**
 * 跟手移动测试
 */
class MovingDrawTest : public ITestDraw {

public:

    MovingDrawTest();

    virtual ~MovingDrawTest();

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_MOVINGDRAWTEST_H
