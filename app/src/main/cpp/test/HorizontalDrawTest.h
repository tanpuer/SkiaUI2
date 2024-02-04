//
// Created by cw on 2022/5/24.
//

#ifndef SKIAUI_HORIZONTALDRAWTEST_H
#define SKIAUI_HORIZONTALDRAWTEST_H


#include "ITestDraw.h"

class HorizontalDrawTest : public ITestDraw {

public:

    HorizontalDrawTest();

    virtual ~HorizontalDrawTest();

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_HORIZONTALDRAWTEST_H
