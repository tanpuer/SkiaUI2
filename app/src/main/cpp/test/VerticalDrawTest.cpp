//
// Created by cw on 2022/5/24.
//

#include "VerticalDrawTest.h"

VerticalDrawTest::VerticalDrawTest() {

}

VerticalDrawTest::~VerticalDrawTest() {

}

void VerticalDrawTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    FlexboxLayout _root;
    _root.setLayoutParams(LayoutParams::makeExactlyLayoutParams(width, height));
    _root.setFlexWrap(YGWrapWrap);
    _root.setFlexDirection(YGFlexDirectionColumn);
    _root.setJustifyContent(YGJustifyCenter);
    _root.setAlignItems(YGAlignCenter);
    _root.setAlignContent(YGAlignCenter);
    _root.setStrokeWidth(10);
    _root.setStyle(SkPaint::kStroke_Style);
    _root.setBackgroundColor(SK_ColorBLUE);
    {
        auto view = new View();
        view->setConfig(_root.config);
        view->setBackgroundColor(SK_ColorRED);
        view->setStyle(SkPaint::kFill_Style);
        view->setCornerRadius(30);
        auto viewLayoutParams = LayoutParams::makeExactlyLayoutParams(300, 200);
        viewLayoutParams->setMargin({0, 0, 0, 0});
        _root.addView(view, viewLayoutParams);

        auto textView = new TextView();
        textView->setConfig(_root.config);
        textView->setText(SkString("Hello Code Boy"));
        textView->setTextColor(SK_ColorBLACK);
        textView->setTextSize(60);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
//    textView->setAlignSelf(YGAlignFlexEnd);
        auto tvLayoutParams = LayoutParams::makeWrapContent(true, true);
        tvLayoutParams->setMargin(50);
        _root.addView(textView, tvLayoutParams);

        auto imageView = new ImageView();
        imageView->setConfig(_root.config);
        imageView->setSource("/sdcard/cowboy.png");
        imageView->setCornerRadius(30);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorRED);
        imageView->setStrokeWidth(10);
        _root.addView(imageView, LayoutParams::makeWrapContent(true, true));
    }

    auto rootWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto rootHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    _root.measure(rootWidthSpec, rootHeightSpec);
    _root.layout(0, 0, width, height);
    _root.draw(canvas);
}
