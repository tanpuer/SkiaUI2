//
// Created by cw on 2022/5/24.
//

#include "HorizontalDrawTest.h"

HorizontalDrawTest::HorizontalDrawTest() {
}

HorizontalDrawTest::~HorizontalDrawTest() {

}

void HorizontalDrawTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
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
        {
            auto _root1 = new FlexboxLayout();
            _root1->setConfig(config);
            _root1->setFlexWrap(YGWrapWrap);
            _root1->setFlexDirection(YGFlexDirectionRow);
            _root1->setJustifyContent(YGJustifyCenter);
            _root1->setAlignItems(YGAlignCenter);
            _root1->setAlignContent(YGAlignCenter);
            _root1->setStrokeWidth(10);
            _root1->setStyle(SkPaint::kStroke_Style);
            _root1->setBackgroundColor(SK_ColorGREEN);
            {
                auto view = new MovingView();
                view->setConfig(_root1->config);
                view->setBackgroundColor(SK_ColorRED);
                view->setStyle(SkPaint::kFill_Style);
                view->setCornerRadius(30);
                auto viewLayoutParams = LayoutParams::makeExactlyLayoutParams(200, 200);
                viewLayoutParams->setMargin({0, 0, 0, 0});
                _root1->addView(view, viewLayoutParams);

                auto textView = new TextView();
                textView->setConfig(_root1->config);
                textView->setText(SkString("Code Boy"));
                textView->setTextColor(SK_ColorBLACK);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
//        textView->setAlignSelf(YGAlignCenter);
                auto tvLayoutParams = LayoutParams::makeWrapContent(true, true);
                tvLayoutParams->setMargin(50);
                _root1->addView(textView, tvLayoutParams);

                auto imageView = new ImageView();
                imageView->setConfig(_root1->config);
                imageView->setSource("/sdcard/cowboy.png");
                imageView->setCornerRadius(30);
                imageView->setStyle(SkPaint::kStroke_Style);
                imageView->setBackgroundColor(SK_ColorRED);
                imageView->setStrokeWidth(10);
                _root1->addView(imageView, LayoutParams::makeWrapContent(true, true));

                root->addView(_root1, LayoutParams::makeExactlyWidth(width));
            }
        }

        {
            auto _root1 = new FlexboxLayout();
            _root1->setConfig(config);
            _root1->setFlexWrap(YGWrapWrap);
            _root1->setFlexDirection(YGFlexDirectionRow);
            _root1->setJustifyContent(YGJustifyCenter);
            _root1->setAlignItems(YGAlignCenter);
            _root1->setAlignContent(YGAlignCenter);
            _root1->setStrokeWidth(10);
            _root1->setStyle(SkPaint::kStroke_Style);
            _root1->setBackgroundColor(SK_ColorGREEN);
            {
                auto view = new View();
                view->setConfig(_root1->config);
                view->setBackgroundColor(SK_ColorYELLOW);
                view->setStyle(SkPaint::kFill_Style);
                view->setCornerRadius(30);
                auto viewLayoutParams = LayoutParams::makeExactlyLayoutParams(300, 200);
                viewLayoutParams->setMargin({0, 0, 0, 0});
                _root1->addView(view, viewLayoutParams);

                auto textView = new TextView();
                textView->setConfig(_root1->config);
                textView->setText(SkString("Code Boy"));
                textView->setTextColor(SK_ColorBLACK);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
//        textView->setAlignSelf(YGAlignCenter);
                auto tvLayoutParams = LayoutParams::makeWrapContent(true, true);
                tvLayoutParams->setMargin(50);
                _root1->addView(textView, tvLayoutParams);

                auto textView1 = new TextView();
                textView1->setConfig(_root1->config);
                textView1->setText(SkString("Code Boy"));
                textView1->setTextColor(SK_ColorBLACK);
                textView1->setTextSize(60);
                textView1->setBackgroundColor(SK_ColorRED);
                textView1->setStyle(SkPaint::kStroke_Style);
                auto tvLayoutParams1 = LayoutParams::makeWrapContent(true, true);
                tvLayoutParams->setMargin(50);
                _root1->addView(textView1, tvLayoutParams1);

                auto imageView = new ImageView();
                imageView->setConfig(_root1->config);
                imageView->setSource("/sdcard/cowboy.png");
                imageView->setCornerRadius(30);
                imageView->setStyle(SkPaint::kStroke_Style);
                imageView->setBackgroundColor(SK_ColorRED);
                imageView->setStrokeWidth(10);
                _root1->addView(imageView, LayoutParams::makeWrapContent(true, true));
//                auto lp = LayoutParams::makeExactlyWidth(width);
                auto lp = LayoutParams::makeWrapContent(true, true);
                lp->setMargin({0, 200, 0, 0});
                root->addView(_root1, lp);
            }

        }
    }
    auto rootWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto rootHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    root->measure(rootWidthSpec, rootHeightSpec);
    root->layout(0, 0, width, height);
    root->draw(canvas);

}
