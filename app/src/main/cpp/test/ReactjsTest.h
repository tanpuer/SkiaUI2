#pragma once

#include "JavascriptTest.h"

namespace HYSkiaUI {

class ReactjsTest : public JavascriptTest {

public:

    ReactjsTest() = default;

    ~ReactjsTest() = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

private:

    bool createFlag = false;

};

}
