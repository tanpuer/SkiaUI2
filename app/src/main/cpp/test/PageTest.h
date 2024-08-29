#pragma once

#include "ITestDraw.h"
#include "Page.h"

class PageTest : public ITestDraw {

public:

    PageTest() = default;

    ~PageTest() = default;

    void performAnimations(int width, int height) override;

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};
