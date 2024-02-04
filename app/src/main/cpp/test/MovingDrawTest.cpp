//
// Created by ByteDance on 2022/6/22.
//

#include "MovingDrawTest.h"

MovingDrawTest::MovingDrawTest() {

}

MovingDrawTest::~MovingDrawTest() {

}

void MovingDrawTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        root = new FlexboxLayout();
        config = YGConfigNew();
        root->setConfig(config);
        root->setLayoutParams(LayoutParams::makeExactlyLayoutParams(width, height));
        root->setFlexWrap(YGWrapWrap);
        root->setFlexDirection(YGFlexDirectionColumn);
        root->setJustifyContent(YGJustifyCenter);
        root->setAlignItems(YGAlignCenter);
        root->setAlignContent(YGAlignCenter);
        root->setStrokeWidth(10);
        root->setStyle(SkPaint::kStroke_Style);
        root->setBackgroundColor(SK_ColorBLUE);

        auto view = new MovingView();
        view->setConfig(root->config);
        view->setBackgroundColor(SK_ColorRED);
        view->setStyle(SkPaint::kFill_Style);
        view->setCornerRadius(30);
        auto viewLayoutParams = LayoutParams::makeExactlyLayoutParams(200, 200);
        viewLayoutParams->setMargin({0, 0, 0, 0});
        root->addView(view, viewLayoutParams);

        {
            auto view = new MovingView();
            view->setConfig(root->config);
            view->setBackgroundColor(SK_ColorBLUE);
            view->setStyle(SkPaint::kFill_Style);
            view->setCornerRadius(30);
            auto viewLayoutParams = LayoutParams::makeExactlyLayoutParams(200, 200);
            viewLayoutParams->setMargin({0, 30, 0, 0});
            root->addView(view, viewLayoutParams);
        }
    }
    auto rootWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto rootHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    root->measure(rootWidthSpec, rootHeightSpec);
    root->layout(0, 0, width, height);
    root->draw(canvas);
}
