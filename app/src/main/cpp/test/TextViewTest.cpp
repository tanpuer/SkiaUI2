//
// Created by cw on 2022/7/2.
//

#include "TextViewTest.h"

TextViewTest::TextViewTest() = default;

TextViewTest::~TextViewTest() = default;

void TextViewTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        root = new FlexboxLayout();
        config = YGConfigNew();
        root->setConfig(config);
        root->setLayoutParams(LayoutParams::makeExactlyLayoutParams(width, height));
        root->setFlexWrap(YGWrapNoWrap);
        root->setFlexDirection(YGFlexDirectionColumn);
        root->setJustifyContent(YGJustifyFlexStart);
        root->setAlignItems(YGAlignCenter);
        root->setAlignContent(YGAlignCenter);
        root->setStrokeWidth(10);
        root->setStyle(SkPaint::kStroke_Style);
        root->setBackgroundColor(SK_ColorBLUE);

        {
            auto root1 = new FlexboxLayout();
            config = YGConfigNew();
            root1->setConfig(config);
            root1->setLayoutParams(LayoutParams::makeWrapContent());
            root1->setFlexWrap(YGWrapWrap);
            root1->setFlexDirection(YGFlexDirectionColumn);
            root1->setJustifyContent(YGJustifyCenter);
            root1->setAlignItems(YGAlignCenter);
            root1->setAlignContent(YGAlignCenter);
            root1->setStrokeWidth(4);
            root1->setStyle(SkPaint::kStroke_Style);
            root1->setBackgroundColor(SK_ColorRED);

            {
                auto textView = new TextView();
                textView->setConfig(root1->config);
                textView->setText(SkString(
                        "ضخمة ص ،😁😂🤣ضضض ؤ،،😗😗😍😋شسي،😗😁😁ؤرى،😗😃😄😍ببب،🥰😅🥰🥰🥰ثيلااتنabcdefghijABCDEFGHIJKLMNOPQRSTUVWXYZabcdefg中文"));
                textView->setTextColor(SK_ColorBLACK);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                textView->setDecoration(kLineThrough);
                textView->setDecorationStyle(kSolid);
                textView->setDecorationThicknessMultiplier(3);
                textView->setDecorationColor(SK_ColorRED);
                auto tvLayoutParams = LayoutParams::makeExactlyWidth(width);
                tvLayoutParams->setMargin(50);
                root1->addView(textView, tvLayoutParams);
            }

            {
                auto textView = new TextView();
                textView->setConfig(root1->config);
                textView->setText(SkString("这是阴影测试"));
                textView->setTextColor(SK_ColorGREEN);
                textView->setTextSize(100);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                textView->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
                auto tvLayoutParams = LayoutParams::makeExactlyLayoutParams(300, 300);
                tvLayoutParams->setMargin(50);
                root1->addView(textView, tvLayoutParams);
            }

            {
                auto textView = new TextView();
                textView->setConfig(root1->config);
                textView->setText(SkString("abcdefghijklmn\nABCDEFGHIJKLMN"));
                textView->setTextColor(SK_ColorGREEN);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                auto tvLayoutParams = LayoutParams::makeExactlyLayoutParams(300, 100);
                tvLayoutParams->setMargin(50);
                root1->addView(textView, tvLayoutParams);
            }

            root->addView(root1, LayoutParams::makeExactlyWidth(width));
        }

        {
            auto root1 = new FlexboxLayout();
            config = YGConfigNew();
            root1->setConfig(config);
            root1->setFlexWrap(YGWrapWrap);
            root1->setFlexDirection(YGFlexDirectionRow);
            root1->setJustifyContent(YGJustifyFlexStart);
            root1->setAlignItems(YGAlignCenter);
            root1->setAlignContent(YGAlignAuto);
            root1->setStrokeWidth(10);
            root1->setStyle(SkPaint::kStroke_Style);
            root1->setBackgroundColor(SK_ColorYELLOW);

            {
                auto textView = new TextView();
                textView->setConfig(root1->config);
                textView->setText(SkString("horizontal test"));
                textView->setTextColor(SK_ColorBLACK);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                auto tvLayoutParams = LayoutParams::makeWrapContent();
                tvLayoutParams->setMargin(50);
                root1->addView(textView, tvLayoutParams);
            }

            {
                auto textView = new TextView();
                textView->setConfig(root1->config);
                textView->setText(SkString("abcdefghijklmnopqrstuvw"));
                textView->setTextColor(SK_ColorGREEN);
                textView->setTextSize(100);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                auto tvLayoutParams = LayoutParams::makeWrapContent();
                tvLayoutParams->setMargin(50);
                root1->addView(textView, tvLayoutParams);
            }

            {
                auto textView = new TextView();
                textView->setConfig(root1->config);
                textView->setTextColor(SK_ColorGREEN);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                textView->pushText(TextView::StringBuilder(SkString("这是"), SK_ColorCYAN,
                                                           SkFontStyle(SkFontStyle::kThin_Weight,
                                                                       SkFontStyle::kNormal_Width,
                                                                       SkFontStyle::kUpright_Slant),
                                                           40));
                textView->pushText(TextView::StringBuilder(SkString("StringBuilder"), SK_ColorGREEN,
                                                           SkFontStyle(SkFontStyle::kThin_Weight,
                                                                       SkFontStyle::kNormal_Width,
                                                                       SkFontStyle::kUpright_Slant),
                                                           100));
                textView->pushText(TextView::StringBuilder(SkString("测试"), SK_ColorYELLOW,
                                                           SkFontStyle(SkFontStyle::kThin_Weight,
                                                                       SkFontStyle::kNormal_Width,
                                                                       SkFontStyle::kUpright_Slant),
                                                           40));
                textView->pushText(TextView::StringBuilder(SkString("demo"), SK_ColorRED,
                                                           SkFontStyle(SkFontStyle::kThin_Weight,
                                                                       SkFontStyle::kNormal_Width,
                                                                       SkFontStyle::kUpright_Slant),
                                                           100));
                auto tvLayoutParams = LayoutParams::makeWrapContent();
                tvLayoutParams->setMargin(50);
                root1->addView(textView, tvLayoutParams);
            }

            auto lp = LayoutParams::makeExactlyWidth(width);
            lp->setMargin(50);
            root->addView(root1, lp);
        }

    }

    auto rootWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto rootHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    root->measure(rootWidthSpec, rootHeightSpec);
    root->layout(0, 0, width, height);
    root->draw(canvas);
}
