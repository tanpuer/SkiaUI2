//
// Created by cw on 2022/5/24.
//

#ifndef SKIAUI_ITESTDRAW_H
#define SKIAUI_ITESTDRAW_H

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

protected:

    ViewGroup *root = nullptr;

    YGConfigRef config;

};


#endif //SKIAUI_ITESTDRAW_H
