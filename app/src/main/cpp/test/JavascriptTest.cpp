#include "JavascriptTest.h"
#include "Page.h"
#include "animationFrame.h"
#include "MeasureTime.h"
#include "performance.h"
#include "timer.h"

namespace HYSkiaUI {

void JavascriptTest::injectViews() {
    viewManager = std::make_unique<ViewManager>(context, v8Runtime);
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
    v8Runtime->injectObject(v8Runtime->global(), "console", consoleMap, {});
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
    v8Runtime->injectObject(v8Runtime->global(), "performance", performanceMap, {});
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
        v8Runtime = std::make_shared<V8Runtime>();
        context->setV8Runtime(v8Runtime);
        injectConsole();
//        injectPerformance();
        injectFrameCallback();
        injectTimer();
        injectViews();
        injectSize(width, height);
    }
}

}
