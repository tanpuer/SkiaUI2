//
// Created by ByteDance on 2022/7/26.
//

#include <ScrollView.h>
#include <ProgressBar.h>
#include "ScrollViewTest.h"
#include "native_log.h"
#include "Button.h"

ScrollViewTest::ScrollViewTest() = default;

ScrollViewTest::~ScrollViewTest() = default;

void ScrollViewTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        root = new ScrollView();
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

        auto shaderView = new View();
        shaderView->setConfig(root->config);
//        shaderView->setBackgroundColor(SK_ColorGRAY);
        shaderView->setShaderPath("sincos.glsl");
        root->addView(shaderView, LayoutParams::makeExactlyLayoutParams(1080, 520));

        for (int i = 0; i < 10; ++i) {
            {
                auto progressBar = new ProgressBar();
                progressBar->setConfig(root->config);
                progressBar->setBarColor(SK_ColorRED);
                progressBar->setBackgroundColor(SK_ColorGRAY);
                progressBar->setStrokeWidth(10.0);
                progressBar->setStyle(SkPaint::kStroke_Style);
                root->addView(progressBar, LayoutParams::makeExactlyLayoutParams(200, 200));
            }

            {
                auto progressBar = new ProgressBar();
                progressBar->setConfig(root->config);
                progressBar->setBarColor(SK_ColorRED);
                progressBar->setBackgroundColor(SK_ColorGRAY);
                progressBar->setStrokeWidth(10.0);
                progressBar->setAutoMode(false);
                progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
                progressBar->setProgress(30);
                progressBar->setStyle(SkPaint::kStroke_Style);
                auto lp = LayoutParams::makeExactlyLayoutParams(width, 60);
                lp->setMargin({50, 50, 50, 50});
                root->addView(progressBar, lp);
                progressBar->setProgressCallback([](int progress) {
                    ALOGD("ProgressBar progress: %d", progress)
                });
            }

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

            {
                auto button = new Button();
                button->setConfig(root->config);
                button->setText(SkString("Button"));
                button->setTextSize(60);
                button->setCornerRadius(20);
                button->setBackgroundColor(SK_ColorRED);
                button->setTextColor(SK_ColorBLACK);
                button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
                auto btnLayoutParams = LayoutParams::makeWrapContent();
                btnLayoutParams->setMargin(50);
                root->addView(button, btnLayoutParams);
                button->setOnClickListener([](View *view) {
                    ALOGD("setOnClickListener perform %s", view->name())
                });
            }

            {
                auto textView = new TextView();
                textView->setConfig(root->config);
                textView->setText(SkString("horizontal test"));
                textView->setTextColor(SK_ColorBLACK);
                textView->setTextSize(60);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                auto tvLayoutParams = LayoutParams::makeWrapContent();
                tvLayoutParams->setMargin(50);
                root->addView(textView, tvLayoutParams);
            }

            {
                auto textView = new TextView();
                textView->setConfig(root->config);
                textView->setText(SkString("abcdefghijklmnopqrstuvw"));
                textView->setTextColor(SK_ColorGREEN);
                textView->setTextSize(100);
                textView->setBackgroundColor(SK_ColorRED);
                textView->setStyle(SkPaint::kStroke_Style);
                auto tvLayoutParams = LayoutParams::makeWrapContent();
                tvLayoutParams->setMargin(50);
                root->addView(textView, tvLayoutParams);
            }

            {
                auto textView = new TextView();
                textView->setConfig(root->config);
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
                root->addView(textView, tvLayoutParams);
                auto listener = [](int l, int t, int r, int b) -> void {
//                    ALOGD("LayoutCallback result %d %d %d %d", l, t, r, b)
                };
                textView->setLayoutCallback(listener);
            }

            {
                auto imageView = new ImageView();
                imageView->setConfig(root->config);
                imageView->setSource("raining.png");
                imageView->setCornerRadius(100);
                imageView->setScaleType(ImageView::ScaleType::CenterCrop);
                imageView->setStyle(SkPaint::kStroke_Style);
                imageView->setBackgroundColor(SK_ColorRED);
                imageView->setStrokeWidth(2);
                imageView->blur(2.0f * i);
                auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
                lp->setMargin({0, 100, 0, 0});
                root->addView(imageView, lp);
            }
        }

    }

    auto rootWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto rootHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    root->measure(rootWidthSpec, rootHeightSpec);
    root->layout(0, 0, width, height);
    root->draw(canvas);

    reinterpret_cast<ScrollView *>(root)->addScrollCallback([](float dx, float dy) -> void {
//        ALOGD("ScrollView scroll dx: %f, dy: %f", dx, dy)
    });
}
