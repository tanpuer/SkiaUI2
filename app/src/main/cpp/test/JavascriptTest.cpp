#include "JavascriptTest.h"
#include "Page.h"
#include "console.h"
#include "animationFrame.h"
#include "MeasureTime.h"
#include "performance.h"
#include "timer.h"
#include "backPressed.h"

namespace HYSkiaUI {

void JavascriptTest::injectViews() {
    viewManager = std::make_unique<ViewManager>(context);
    viewManager->registerHYViews();
}

void JavascriptTest::injectSize(int width, int height) {
    v8Runtime->injectNumber("innerWidth", width);
    v8Runtime->injectNumber("innerHeight", height);
}

void JavascriptTest::invokeFrameCallback() {
    for (const auto &item: frameCallbackMap) {
        const int argc = 0;
        v8::Local<v8::Value> argv[argc] = {};
        v8Runtime->performFunction(item.second, argc, argv);
    }
}

void JavascriptTest::injectConsole() {
    std::map<std::string, v8::FunctionCallback> consoleMap(
            {
                    {"log",   logCallback},
                    {"error", errorCallback},
                    {"info",  infoCallback},
                    {"warn",  warnCallback},
            }
    );
    v8Runtime->injectObject(v8Runtime->global(), "console", consoleMap, {}, this);
}


void JavascriptTest::injectFrameCallback() {
    v8Runtime->injectFunction("requestAnimationFrame", requestAnimationFrameCallback, this);
    v8Runtime->injectFunction("cancelAnimationFrame", cancelAnimationFrameCallback, this);
}

void JavascriptTest::injectPerformance() {
    std::map<std::string, v8::FunctionCallback> performanceMap(
            {
                    {"now", nowCallback}
            }
    );
    v8Runtime->injectObject(v8Runtime->global(), "performance", performanceMap, {}, this);
}

void JavascriptTest::injectTimer() {
    v8Runtime->injectFunction("setTimeout", setTimeout, this);
    v8Runtime->injectFunction("clearTimeout", clearTimeout, this);
}

std::shared_ptr<V8Runtime> &JavascriptTest::getV8Runtime() {
    return v8Runtime;
}

void JavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (!injected) {
        injected = true;
        v8Runtime = context->initV8Runtime();
        injectConsole();
        injectPerformance();
        injectFrameCallback();
        injectTimer();
        injectViews();
        injectBackPressedCallback();
//        1. adb forward tcp:8080 tcp:8080
//        2. run app
//        3. open in chrome:
//              devtools://devtools/bundled/inspector.html?experiments=true&v8only=true&ws=0.0.0.0:8080
//        inspector = std::make_shared<Inspector>(context, 8080);
//        inspector->startAgent();
    }
    if (this->width != width || this->height != height) {
        this->width = width;
        this->height = height;
        injectSize(width, height);
    }
}

void JavascriptTest::injectBackPressedCallback() {
    v8Runtime->injectFunctionToSkiaUI("setBackPressedCallback", backPressedCallback, this);
    getContext()->setBackPressedInterceptor([this]() {
        for (auto &item: backPressCallbackMap) {
            const int argc = 0;
            v8::Local<v8::Value> argv[argc] = {};
            v8Runtime->performFunction(item.second, argc, argv);
        }
    });
}

std::shared_ptr<Inspector> &JavascriptTest::getInspector() {
    return inspector;
}

}
