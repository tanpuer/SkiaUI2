#pragma once

#include "core/SkCanvas.h"
#include "FlexboxLayout.h"
#include "TextView.h"
#include "ImageView.h"
#include "MovingView.h"

/**
 * 各种布局测试用例
 */
class ITestDraw {

public:

    ITestDraw() {};

    virtual ~ITestDraw() {
        delete root;
        YGConfigFree(config);
    };

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) = 0;

    virtual View *getRootView() {
        return root;
    };

    virtual void setContext(std::shared_ptr<SkiaUIContext> context) {
        this->context = context;
    }

protected:

    ViewGroup *root = nullptr;

    YGConfigRef config;

    std::shared_ptr<SkiaUIContext> context;

};
