#include "SimpleJavascriptTest.h"

namespace HYSkiaUI {

void SimpleJavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    JavascriptTest::doDrawTest(drawCount, canvas, width, height);
    MeasureTime measureTime("Javascript draw");
    if (!createFlag) {
        createFlag = true;
        auto jsBuffer = context->getAssetManager()->readFile("test.js");
        v8Runtime->evaluateJavaScript(jsBuffer, "test.js");
    }
    performAnimations(width, height);
    invokeFrameCallback();
    context->getPageStackManager()->removeDestroyedPage();
    for (const auto &item: context->getPageStackManager()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->drawOneFrame(drawCount);
        item->measure();
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}

}
