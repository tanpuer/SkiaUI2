//
// Created by cw on 2022/7/1.
//

#ifndef SKIAUI_IMAGEVIEWTEST_H
#define SKIAUI_IMAGEVIEWTEST_H


#include "ITestDraw.h"

class ImageViewTest : public ITestDraw {

public:

    ImageViewTest();

    ~ImageViewTest();

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;


};


#endif //SKIAUI_IMAGEVIEWTEST_H
