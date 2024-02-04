//
// Created by ByteDance on 2022/8/8.
//

#include <ProgressBar.h>
#include "ProgressBarDrawTest.h"
#include "FlexboxLayout.h"

void ProgressBarDrawTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
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
            auto progressBar = new ProgressBar();
            progressBar->setConfig(root->config);
            progressBar->setBarColor(SK_ColorRED);
            progressBar->setBackgroundColor(SK_ColorGRAY);
            progressBar->setStrokeWidth(10.0);
            progressBar->setStyle(SkPaint::kStroke_Style);
            SkColor colors[3] = {SK_ColorRED, SK_ColorYELLOW, SK_ColorBLUE};
            progressBar->setGradientBarColor(colors, 3);
            root->addView(progressBar, LayoutParams::makeExactlyLayoutParams(200, 200));
        }

        {
            auto progressBar = new ProgressBar();
            progressBar->setConfig(root->config);
            progressBar->setBarColor(SK_ColorBLUE);
            progressBar->setBackgroundColor(SK_ColorGRAY);
            progressBar->setStrokeWidth(10.0);
            progressBar->setAutoMode(false);
            progressBar->setProgress(30);
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
            lp->setMargin({50, 100, 50, 0});
            root->addView(progressBar, lp);
            progressBar->setProgressCallback([](int progress) {
                ALOGD("ProgressBar progress: %d", progress)
            });
        }

        {
            auto progressBar = new ProgressBar();
            progressBar->setConfig(root->config);
            progressBar->setBarColor(SK_ColorRED);
            progressBar->setBackgroundColor(SK_ColorGRAY);
            progressBar->setStrokeWidth(10.0);
            progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
            progressBar->setProgress(30);
            progressBar->setStyle(SkPaint::kStroke_Style);
            SkColor colors[3] = {SK_ColorRED, SK_ColorYELLOW, SK_ColorBLUE};
            progressBar->setGradientBarColor(colors, 3);
            auto lp = LayoutParams::makeExactlyLayoutParams(width, 60);
            lp->setMargin({50, 100, 50, 0});
            root->addView(progressBar, lp);
        }
    }

    auto rootWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto rootHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    root->measure(rootWidthSpec, rootHeightSpec);
    root->layout(0, 0, width, height);
    root->draw(canvas);
}

ProgressBarDrawTest::ProgressBarDrawTest() = default;

ProgressBarDrawTest::~ProgressBarDrawTest() = default;
