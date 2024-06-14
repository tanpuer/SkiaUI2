#include "JavascriptTest.h"

JavascriptTest::JavascriptTest() {
    v8Runtime = std::make_shared<V8Runtime>(context);
}

void JavascriptTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {

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

}

void JavascriptTest::injectFrameCallback() {

}
