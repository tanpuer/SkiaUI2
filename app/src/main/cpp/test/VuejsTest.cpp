#include "VuejsTest.h"

namespace HYSkiaUI {

void VuejsTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    JavascriptTest::doDrawTest(drawCount, canvas, width, height);
    if (!createFlag) {
        createFlag = true;
        auto jsBuffer = context->getAssetManager()->readFile("vue_bundle.js");
        v8Runtime->evaluateJavaScript(jsBuffer, "vue_bundle.js");
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
