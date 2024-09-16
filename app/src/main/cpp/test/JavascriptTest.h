#pragma once

#include "ITestDraw.h"
#include "V8Runtime.h"
#include "console.h"
#include "ViewManager.h"

class JavascriptTest : public ITestDraw {

public:

    JavascriptTest() = default;

    ~JavascriptTest() override = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

public:

    int FRAME_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> frameCallbackMap;

private:

    void injectNodeApi();

    void injectConsole();

    void injectFrameCallback();

    void injectPerformance();

private:

    void injectViews();

    void injectSize(int width, int height);

    void invokeFrameCallback();

    void createRoot(int width, int height);

private:

    std::shared_ptr<V8Runtime> v8Runtime;

    std::shared_ptr<ViewManager> viewManager;

};
