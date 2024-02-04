//
// Created by ByteDance on 2022/8/8.
//

#ifndef SKIAUI_PROGRESSBARDRAWTEST_H
#define SKIAUI_PROGRESSBARDRAWTEST_H


#include "ITestDraw.h"

class ProgressBarDrawTest: public ITestDraw {

public:

    ProgressBarDrawTest();

    virtual ~ProgressBarDrawTest();

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_PROGRESSBARDRAWTEST_H
