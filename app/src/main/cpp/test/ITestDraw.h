#pragma once

#include "core/SkCanvas.h"
#include "FlexboxLayout.h"
#include "TextView.h"
#include "ImageView.h"
#include "MovingView.h"
#include "Page.h"

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

    virtual std::shared_ptr<SkiaUIContext> &getContext() {
        return context;
    }

    virtual void onShow() {
        auto page = context->getPageStackManager()->back();
        if (page != nullptr) {
            page->onShow();
        }
    }

    virtual void onHide() {
        auto page = context->getPageStackManager()->back();
        if (page != nullptr) {
            page->onHide();
        }
    }

protected:

    ViewGroup *root = nullptr;

    YGConfigRef config;

    std::shared_ptr<SkiaUIContext> context;

};
