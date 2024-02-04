//
// Created by ByteDance on 2022/7/26.
//

#ifndef SKIAUI_SCROLLVIEWTEST_H
#define SKIAUI_SCROLLVIEWTEST_H


#include "ITestDraw.h"

class ScrollViewTest: public ITestDraw {

public:

    ScrollViewTest();

    virtual ~ScrollViewTest();

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_SCROLLVIEWTEST_H
