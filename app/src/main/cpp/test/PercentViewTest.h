//
// Created by cw on 2022/5/30.
//

#ifndef SKIAUI_PERCENTVIEWTEST_H
#define SKIAUI_PERCENTVIEWTEST_H


#include "ITestDraw.h"

class PercentViewTest : public ITestDraw{

public:

    PercentViewTest();

    ~PercentViewTest();

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_PERCENTVIEWTEST_H
