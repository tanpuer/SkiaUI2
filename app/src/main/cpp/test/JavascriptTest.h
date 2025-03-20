#pragma once

#include "ITestDraw.h"
#include "V8Runtime.h"
#include "ViewManager.h"
#include "inspect/Inspector.h"

namespace HYSkiaUI {

class JavascriptTest : public ITestDraw {

public:

    JavascriptTest() = default;

    ~JavascriptTest() {
        auto page = context->getPageStackManager()->pop();
        delete page;
    };

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    std::shared_ptr<V8Runtime>& getV8Runtime();

    std::shared_ptr<Inspector>& getInspector();

public:

    int FRAME_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> frameCallbackMap;

    int TIMER_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> timerCallbackMap;

    int BACK_PRESSED_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> backPressCallbackMap;

protected:

    void injectConsole();

    void injectFrameCallback();

    void injectPerformance();

    void injectTimer();

    bool injected = false;

    int width = 0;
    int height = 0;

protected:

    void injectViews();

    void injectSize(int width, int height);

    void invokeFrameCallback();

    void injectBackPressedCallback();

protected:

    std::shared_ptr<V8Runtime> v8Runtime;

    std::shared_ptr<ViewManager> viewManager;

    std::shared_ptr<Inspector> inspector;

};

}
