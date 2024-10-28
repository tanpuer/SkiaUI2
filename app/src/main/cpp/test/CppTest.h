#pragma once

#include "ITestDraw.h"
#include "Page.h"

class CppTest : public ITestDraw {

public:

    CppTest() = default;

    ~CppTest() = default;

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

};
