//
// Created by banma-3412 on 2024/3/21.
//

#include "PageTest.h"
#include "Page.h"
#include "ScrollView.h"
#include "ShaderView.h"
#include "SVGView.h"
#include "CanvasTest.h"
#include "ProgressBar.h"
#include "Button.h"
#include "PageStackManager.h"

void PageTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        auto page = initPage(width, height);
        root = page;
        initChildren(drawCount, root, width, height);
        PageStackManager::getInstance()->push(page);
        page->enterFromRight(Page::EnterExitInfo(width, 0));
//        page->enterFromBottom(Page::EnterExitInfo(height, 0));
    }
    auto scrollViewWidthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto scrollViewHeightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);

    for (const auto &item: PageStackManager::getInstance()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->measure(scrollViewWidthSpec, scrollViewHeightSpec);
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}

void PageTest::initChildren(int drawCount, ViewGroup *root, int width, int height) {
    auto scrollView = new ScrollView();
    config = YGConfigNew();
    scrollView->setConfig(config);
    scrollView->setFlexWrap(YGWrapNoWrap);
    scrollView->setFlexDirection(YGFlexDirectionColumn);
    scrollView->setJustifyContent(YGJustifyFlexStart);
    scrollView->setAlignItems(YGAlignCenter);
    scrollView->setAlignContent(YGAlignCenter);
    scrollView->setStyle(SkPaint::kFill_Style);
    scrollView->setBackgroundColor(SK_ColorWHITE);
    root->addView(scrollView, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto view = new View();
        view->setConfig(scrollView->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        scrollView->addView(view, LayoutParams::makeExactlyLayoutParams(200, 200));
        view->setOnClickListener([this, width, height, drawCount](View *view) {
            auto page = initPage(width, height);
            initChildren(drawCount, page, width, height);
            PageStackManager::getInstance()->push(page);
            page->enterFromRight(Page::EnterExitInfo(width, 0));
//            page->enterFromBottom(Page::EnterExitInfo(height, 0));
        });
    }

    {
        auto view = new View();
        view->setConfig(scrollView->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorCYAN);
        colors.push_back(SK_ColorMAGENTA);
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorCYAN);
        view->setSwiperGradient(colors);
        view->setCornerRadius(20);
        view->setBlurMask(kNormal_SkBlurStyle, 10);
        auto lp = LayoutParams::makeExactlyLayoutParams(400, 400);
        lp->setMargin({0, 50, 0, 50});
        scrollView->addView(view, lp);
        view->setOnClickListener([this, width, height](View *view) {
            auto page = PageStackManager::getInstance()->back();
            if (page == nullptr) {
                ALOGE("pop failed due to empty pages")
                return;
            }
            page->exitToLeft(Page::EnterExitInfo(0, width));
//            page->exitToTop(Page::EnterExitInfo(0, height));
        });
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setConfig(scrollView->config);
        shaderView->setShaderPath("sincos.glsl");
        scrollView->addView(shaderView, LayoutParams::makeExactlyLayoutParams(1080, 520));
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setConfig(scrollView->config);
        shaderView->setShaderPath("raining.glsl", {"raining.png"});
        auto lp = LayoutParams::makeExactlyLayoutParams(1080, 520);
        lp->setMargin({0, 50, 0, 0});
        scrollView->addView(shaderView, lp);
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setConfig(scrollView->config);
        shaderView->setShaderPath("gl_transition_window_slice.glsl",
                                  {"transition1.png", "transition2.png"});
        shaderView->setCustomUniforms("count", 10.0);
        shaderView->setCustomUniforms("smoothness", 0.5);
        auto lp = LayoutParams::makeExactlyLayoutParams(512, 400);
        lp->setMargin({0, 50, 0, 0});
        scrollView->addView(shaderView, lp);
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setConfig(scrollView->config);
        shaderView->setShaderPath("gl_transition_cross_zoom.glsl",
                                  {"transition1.png", "transition2.png"});
        shaderView->setCustomUniforms("strength", 0.4);
        auto lp = LayoutParams::makeExactlyLayoutParams(512, 400);
        lp->setMargin({0, 50, 0, 0});
        scrollView->addView(shaderView, lp);
    }

    {
        auto imageView = new ImageView();
        imageView->setConfig(scrollView->config);
        imageView->setSource("bird.gif");
        imageView->setScaleType(ImageView::ScaleType::FitCenter);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorRED);
        imageView->setStrokeWidth(2);
        auto lp = LayoutParams::makeExactlyLayoutParams(800, 500);
        lp->setMargin({0, 100, 0, 0});
        scrollView->addView(imageView, lp);
    }

    {
        auto svgView = new SVGView();
        svgView->setConfig(scrollView->config);
        svgView->setSource("tiger.svg");
        svgView->setStyle(SkPaint::kStroke_Style);
        svgView->setBackgroundColor(SK_ColorRED);
        svgView->setStrokeWidth(2);
        svgView->setXY(100, 100);
        auto lp = LayoutParams::makeExactlyLayoutParams(800, 800);
        lp->setMargin({0, 100, 0, 0});
        scrollView->addView(svgView, lp);
    }

    {
        auto canvasTest = new CanvasTest();
        canvasTest->setConfig(scrollView->config);
        canvasTest->setCircleSize(200);
        canvasTest->setStyle(SkPaint::kStroke_Style);
        canvasTest->setBackgroundColor(SK_ColorRED);
        canvasTest->setStrokeWidth(2);
        auto lp = LayoutParams::makeExactlyLayoutParams(600, 800);
        lp->setMargin({0, 50, 0, 50});
        scrollView->addView(canvasTest, lp);
    }

    {
        auto progressBar = new ProgressBar();
        progressBar->setConfig(scrollView->config);
        progressBar->setBarColor(SK_ColorRED);
        progressBar->setBackgroundColor(SK_ColorGRAY);
        progressBar->setStrokeWidth(10.0);
        progressBar->setAutoMode(false);
        progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
        progressBar->setProgress(30);
        progressBar->setStyle(SkPaint::kStroke_Style);
        auto lp = LayoutParams::makeExactlyLayoutParams(width, 60);
        lp->setMargin({50, 50, 50, 50});
        scrollView->addView(progressBar, lp);
        progressBar->setProgressCallback([](int progress) {
            ALOGD("ProgressBar progress: %d", progress)
        });
    }

    {
        auto view = new MovingView();
        view->setConfig(scrollView->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setStyle(SkPaint::kFill_Style);
        view->setCornerRadius(30);
        auto viewLayoutParams = LayoutParams::makeExactlyLayoutParams(200, 200);
        viewLayoutParams->setMargin({0, 30, 0, 0});
        scrollView->addView(view, viewLayoutParams);
    }

    {
        auto button = new Button();
        button->setConfig(scrollView->config);
        button->setText(SkString("Button"));
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->setBackgroundColor(SK_ColorRED);
        button->setTextColor(SK_ColorBLACK);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        auto btnLayoutParams = LayoutParams::makeWrapContent();
        btnLayoutParams->setMargin(50);
        scrollView->addView(button, btnLayoutParams);
        button->setOnClickListener([](View *view) {
            ALOGD("setOnClickListener perform %s", view->name())
        });
    }

    {
        auto textView = new TextView();
        textView->setConfig(scrollView->config);
        textView->setText(SkString("horizontal test"));
        textView->setTextColor(SK_ColorBLACK);
        textView->setTextSize(60);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        auto tvLayoutParams = LayoutParams::makeWrapContent();
        tvLayoutParams->setMargin(50);
        scrollView->addView(textView, tvLayoutParams);
    }

    {
        auto textView = new TextView();
        textView->setConfig(scrollView->config);
        textView->setText(SkString("abcdefghijklmnopqrstuvw"));
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(100);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        auto tvLayoutParams = LayoutParams::makeWrapContent();
        tvLayoutParams->setMargin(50);
        scrollView->addView(textView, tvLayoutParams);
    }

    {
        auto textView = new TextView();
        textView->setConfig(scrollView->config);
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
        scrollView->addView(textView, tvLayoutParams);
    }

    {
        auto imageView = new ImageView();
        imageView->setConfig(scrollView->config);
        imageView->setSource("raining.png");
        imageView->setCornerRadius(200);
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorRED);
        imageView->setStrokeWidth(2);
        imageView->blur(10.0f);
        auto lp = LayoutParams::makeExactlyLayoutParams(400, 400);
        lp->setMargin({0, 100, 0, 0});
        scrollView->addView(imageView, lp);
    }
}

Page *PageTest::initPage(int width, int height) {
    auto page = new Page();
    config = YGConfigNew();
    page->setConfig(config);
    page->setLayoutParams(LayoutParams::makeExactlyLayoutParams(width, height));
    page->setFlexWrap(YGWrapWrap);
    page->setFlexDirection(YGFlexDirectionColumn);
    page->setJustifyContent(YGJustifyCenter);
    page->setAlignItems(YGAlignCenter);
    page->setAlignContent(YGAlignCenter);
    page->setStyle(SkPaint::kFill_Style);
    page->setBackgroundColor(SK_ColorTRANSPARENT);
    return page;
}

View *PageTest::getRootView() {
    auto page = PageStackManager::getInstance()->back();
    SkASSERT(page != nullptr && page->children.size() == 1);
    return page->children[0];
}
