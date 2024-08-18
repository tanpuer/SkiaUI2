#include "JavascriptTest.h"
#include "Page.h"
#include "animationFrame.h"
#include "MeasureTime.h"
#include "performance.h"

void JavascriptTest::setContext(std::shared_ptr<SkiaUIContext> context) {
    MeasureTime measureTime("Javascript init");
    ITestDraw::setContext(context);
    v8Runtime = std::make_shared<V8Runtime>();
    context->setV8Runtime(v8Runtime);
    injectConsole();
    injectFrameCallback();
    injectViews();
    config = YGConfigNew();
    context->setConfigRef(config);
}

void JavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    MeasureTime measureTime("Javascript draw");
    createRoot(width, height);
    invokeFrameCallback();
}

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

void JavascriptTest::createRoot(int width, int height) {
    if (root == nullptr) {
        injectSize(width, height);
        auto jsBuffer = context->getAssetManager()->readFile("test.js");
        v8Runtime->evaluateJavaScript(jsBuffer, "test.js");
        auto result = v8Runtime->callFunction("createRoot", 0, nullptr);
        assert(result->IsObject());
        v8Runtime->enterContext(
                [this, &result, width, height](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
                    auto rootView = v8::Local<v8::External>::Cast(
                            result->ToObject()->GetInternalField(0));
                    auto page = static_cast<Page *>(rootView->Value());
                    root = page;
                });
    }
}

void JavascriptTest::injectNodeApi() {
    injectConsole();
    injectFrameCallback();
    injectPerformance();
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
