#pragma once

#include "ITestDraw.h"
#include "V8Runtime.h"

class JavascriptTest : public ITestDraw {

public:

    JavascriptTest();

    ~JavascriptTest() = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    View *getRootView() override;

private:

    void injectConsole();

    void injectViews();

    void injectFrameCallback();

private:

    std::shared_ptr<V8Runtime> v8Runtime;

};
