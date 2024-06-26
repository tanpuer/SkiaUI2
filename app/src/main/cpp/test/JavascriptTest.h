#pragma once

#include "ITestDraw.h"
#include "V8Runtime.h"
#include "console.h"
#include "ViewManager.h"

class JavascriptTest : public ITestDraw {

public:

    JavascriptTest() = default;

    ~JavascriptTest() override = default;

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    View *getRootView() override;

    virtual void setContext(std::shared_ptr<SkiaUIContext> context) override;

public:

    int FRAME_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> frameCallbackMap;

private:

    Page *initPage(int width, int height);

    void injectConsole();

    void injectViews();

    void injectFrameCallback();

private:

    std::shared_ptr<V8Runtime> v8Runtime;

    std::shared_ptr<ViewManager> viewManager;

};
