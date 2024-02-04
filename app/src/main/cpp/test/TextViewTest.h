//
// Created by cw on 2022/7/2.
//

#ifndef SKIAUI_TEXTVIEWTEST_H
#define SKIAUI_TEXTVIEWTEST_H


#include "ITestDraw.h"
#include "core/SkCanvas.h"

class TextViewTest: public ITestDraw{

public:

    TextViewTest();

    ~TextViewTest();

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height);

};


#endif //SKIAUI_TEXTVIEWTEST_H
