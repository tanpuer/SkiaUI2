//
// Created by ByteDance on 2022/8/4.
//

#ifndef SKIAUI_LISTVIEWTEST_H
#define SKIAUI_LISTVIEWTEST_H


#include "ITestDraw.h"

class ListViewTest: public ITestDraw {

public:

    ListViewTest();

    virtual ~ListViewTest();

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};


#endif //SKIAUI_LISTVIEWTEST_H
