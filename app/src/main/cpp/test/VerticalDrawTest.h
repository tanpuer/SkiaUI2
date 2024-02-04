//
// Created by cw on 2022/5/24.
//

#ifndef SKIAUI_VERTICALDRAWTEST_H
#define SKIAUI_VERTICALDRAWTEST_H


#include "ITestDraw.h"

class VerticalDrawTest : public ITestDraw {

public:

    VerticalDrawTest();

    virtual ~VerticalDrawTest();

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_VERTICALDRAWTEST_H
