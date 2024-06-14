#pragma once

#include "ITestDraw.h"
#include "V8Runtime.h"
#include "console.h"

class JavascriptTest : public ITestDraw {

public:

    JavascriptTest();

    ~JavascriptTest() override = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    View *getRootView() override;

public:

    int FRAME_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> frameCallbackMap;

private:

    void injectConsole();

    void injectViews();

    void injectFrameCallback();

private:

    std::shared_ptr<V8Runtime> v8Runtime;

};
