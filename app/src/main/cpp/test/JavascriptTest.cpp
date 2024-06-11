#include "JavascriptTest.h"

JavascriptTest::JavascriptTest() {
    v8Runtime = std::make_shared<V8Runtime>(context);
}

void JavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {

}

View *JavascriptTest::getRootView() {
    return ITestDraw::getRootView();
}
