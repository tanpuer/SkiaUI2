#pragma once

#include "JavascriptTest.h"

namespace HYSkiaUI {

class SimpleJavascriptTest : public JavascriptTest {

public:

    SimpleJavascriptTest() = default;

    ~SimpleJavascriptTest() = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

private:

    bool createFlag = false;

};

}
