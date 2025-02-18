#include "SimpleJavascriptTest.h"

namespace HYSkiaUI {

void SimpleJavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    JavascriptTest::doDrawTest(drawCount, canvas, width, height);
    MeasureTime measureTime("Javascript draw");
    if (root == nullptr) {
        createRoot(width, height);
    }
    performAnimations(width, height);
    invokeFrameCallback();
}

void SimpleJavascriptTest::createRoot(int width, int height) {
    if (root == nullptr) {
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

}
