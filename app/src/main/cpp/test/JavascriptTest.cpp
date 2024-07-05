#include "JavascriptTest.h"
#include "Page.h"
#include "animationFrame.h"

void JavascriptTest::setContext(std::shared_ptr<SkiaUIContext> context) {
    ITestDraw::setContext(context);
    v8Runtime = std::make_shared<V8Runtime>(context);
    injectConsole();
    injectFrameCallback();
    injectViews();
    config = YGConfigNew();
    this->context->setConfigRef(config);
}

void JavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    createRoot(width, height);
    invokeFrameCallback();
//    root->measure();
//    root->layout(0, 0, width, height);
    root->draw(canvas);
}

View *JavascriptTest::getRootView() {
    return ITestDraw::getRootView();
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
    v8Runtime->injectObject(v8Runtime->global(), "console", consoleMap,
                            std::map<std::string, std::string>());
}

void JavascriptTest::injectViews() {
    viewManager = std::make_unique<ViewManager>(context, v8Runtime);
    viewManager->registerHYViews();
}

void JavascriptTest::injectFrameCallback() {
    v8Runtime->injectFunction("requestAnimationFrame", requestAnimationFrameCallback, this);
    v8Runtime->injectFunction("cancelAnimationFrame", cancelAnimationFrameCallback, this);
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
