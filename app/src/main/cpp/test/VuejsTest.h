#pragma once

#include "JavascriptTest.h"

namespace HYSkiaUI {

class VuejsTest : public JavascriptTest {

public:

    VuejsTest() = default;

    ~VuejsTest() = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

private:

    bool createFlag = false;

};

}
