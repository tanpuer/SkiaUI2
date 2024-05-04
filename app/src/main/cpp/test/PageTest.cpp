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
#include "ClockView.h"

void PageTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        auto page = initPage(width, height);
        root = page;
        initChildren(drawCount, root, width, height);
        context->getPageStackManager()->push(page);
        page->enterFromRight(Page::EnterExitInfo(width, 0));
//        page->enterFromBottom(Page::EnterExitInfo(height, 0));
    }
    if (root->getWidth() != width || root->getHeight() != height) {
        root->setWidth(width);
        root->setHeight(height);
    }
    for (const auto &item: context->getPageStackManager()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->measure();
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}

void PageTest::initChildren(int drawCount, ViewGroup *root, int width, int height) {
    auto scrollView = new ScrollView();
    config = YGConfigNew();
    scrollView->setContext(this->context);
    scrollView->setFlexWrap(YGWrapNoWrap);
    scrollView->setFlexDirection(YGFlexDirectionColumn);
    scrollView->setJustifyContent(YGJustifyFlexStart);
    scrollView->setAlignItems(YGAlignCenter);
    scrollView->setAlignContent(YGAlignCenter);
    scrollView->setStyle(SkPaint::kFill_Style);
    scrollView->setBackgroundColor(SK_ColorWHITE);
    scrollView->setFlex(1);
    root->addView(scrollView);

    {
        auto view = new View();
        view->setContext(this->context);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setWidth(200);
        view->setHeight(200);
        scrollView->addView(view);
        view->setOnClickListener([this, width, height, drawCount](View *view) {
            auto page = initPage(width, height);
            initChildren(drawCount, page, width, height);
            context->getPageStackManager()->push(page);
            page->enterFromRight(Page::EnterExitInfo(width, 0));
//            page->enterFromBottom(Page::EnterExitInfo(height, 0));
        });
    }

    {
        auto view = new View();
        view->setContext(this->context);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorCYAN);
        colors.push_back(SK_ColorMAGENTA);
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorCYAN);
        view->setSwiperGradient(colors);
        view->setCornerRadius(20);
        view->setBlurMask(kNormal_SkBlurStyle, 10);
        view->setWidth(400);
        view->setHeight(400);
        view->setMargin({0, 50, 0, 50});
        scrollView->addView(view);
        view->setOnClickListener([this, width, height](View *view) {
            auto page = context->getPageStackManager()->back();
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
        shaderView->setContext(this->context);
        shaderView->setShaderPath("sincos.glsl");
        shaderView->setWidth(1080);
        shaderView->setHeight(520);
        scrollView->addView(shaderView);
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setContext(this->context);
        shaderView->setShaderPath("raining.glsl", {"raining.png"});
        shaderView->setWidth(1080);
        shaderView->setHeight(520);
        shaderView->setMargin({0, 50, 0, 0});
        scrollView->addView(shaderView);
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setContext(this->context);
        shaderView->setShaderPath("gl_transition_window_slice.glsl",
                                  {"transition1.png", "transition2.png"});
        shaderView->setCustomUniforms("count", 10.0);
        shaderView->setCustomUniforms("smoothness", 0.5);
        shaderView->setWidth(512);
        shaderView->setHeight(400);
        shaderView->setMargin({0, 50, 0, 0});
        scrollView->addView(shaderView);
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setContext(this->context);
        shaderView->setShaderPath("gl_transition_cross_zoom.glsl",
                                  {"transition1.png", "transition2.png"});
        shaderView->setCustomUniforms("strength", 0.4);
        shaderView->setWidth(512);
        shaderView->setHeight(400);
        shaderView->setMargin({0, 50, 0, 0});
        scrollView->addView(shaderView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setSource("bird.gif");
        imageView->setScaleType(ImageView::ScaleType::FitCenter);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorRED);
        imageView->setStrokeWidth(2);
        imageView->setWidth(800);
        imageView->setHeight(500);
        imageView->setMargin({0, 100, 0, 0});
        scrollView->addView(imageView);
    }

    {
        auto svgView = new SVGView();
        svgView->setContext(this->context);
        svgView->setSource("tiger.svg");
        svgView->setStyle(SkPaint::kStroke_Style);
        svgView->setBackgroundColor(SK_ColorRED);
        svgView->setStrokeWidth(2);
        svgView->setXY(100, 100);
        svgView->setWidth(800);
        svgView->setHeight(800);
        svgView->setMargin({0, 100, 0, 0});
        scrollView->addView(svgView);
    }

    {
        auto canvasTest = new CanvasTest();
        canvasTest->setContext(this->context);
        canvasTest->setCircleSize(200);
        canvasTest->setStyle(SkPaint::kStroke_Style);
        canvasTest->setBackgroundColor(SK_ColorRED);
        canvasTest->setStrokeWidth(2);
        canvasTest->setWidth(600);
        canvasTest->setHeight(800);
        canvasTest->setMargin({0, 50, 0, 50});
        scrollView->addView(canvasTest);
    }

    {
        auto canvasTest = new ClockView();
        canvasTest->setContext(this->context);
        canvasTest->setWidth(600);
        canvasTest->setHeight(600);
        canvasTest->setMargin({0, 50, 0, 50});
        scrollView->addView(canvasTest);
    }

    {
        auto progressBar = new ProgressBar();
        progressBar->setContext(this->context);
        progressBar->setBarColor(SK_ColorRED);
        progressBar->setBackgroundColor(SK_ColorGRAY);
        progressBar->setStrokeWidth(10.0);
        progressBar->setAutoMode(false);
        progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
        progressBar->setProgress(30);
        progressBar->setStyle(SkPaint::kStroke_Style);
        progressBar->setWidth(width);
        progressBar->setHeight(60);
        progressBar->setMargin({50, 50, 50, 50});
        scrollView->addView(progressBar);
        progressBar->setProgressCallback([](int progress) {
            ALOGD("ProgressBar progress: %d", progress)
        });
    }

    {
        auto view = new MovingView();
        view->setContext(this->context);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setStyle(SkPaint::kFill_Style);
        view->setCornerRadius(30);
        view->setWidth(200);
        view->setHeight(200);
        view->setMargin({0, 30, 0, 0});
        scrollView->addView(view);
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Button"));
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->setBackgroundColor(SK_ColorRED);
        button->setTextColor(SK_ColorBLACK);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        button->setMargin({50, 50, 50, 50});
        button->setWidth(260);
        button->setHeight(100);
        scrollView->addView(button);
        button->setOnClickListener([](View *view) {
            ALOGD("setOnClickListener perform %s", view->name())
        });
    }

    {
        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setText(SkString("horizontal test"));
        textView->setTextColor(SK_ColorBLACK);
        textView->setTextSize(60);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        textView->setMargin({50, 50, 50, 50});
        scrollView->addView(textView);
    }

    {
        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setText(SkString("abcdefghijklmnopqrstuvw"));
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(100);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        textView->setMargin({50, 50, 50, 50});
        scrollView->addView(textView);
    }

    {
        auto textView = new TextView();
        textView->setContext(this->context);
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
        textView->setMargin({50, 50, 50, 50});
        scrollView->addView(textView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setSource("raining.png");
        imageView->setCornerRadius(200);
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorRED);
        imageView->setStrokeWidth(2);
        imageView->blur(10.0f);
        imageView->setWidth(400);
        imageView->setHeight(400);
        imageView->setMargin({0, 100, 0, 0});
        scrollView->addView(imageView);
    }
}

Page *PageTest::initPage(int width, int height) {
    auto page = new Page();
    config = YGConfigNew();
    this->context->setConfigRef(config);
    page->setContext(this->context);
    page->setWidth(width);
    page->setHeight(height);
    page->setFlexWrap(YGWrapWrap);
    page->setFlexDirection(YGFlexDirectionColumn);
    page->setJustifyContent(YGJustifyCenter);
    page->setAlignItems(YGAlignCenter);
    page->setAlignContent(YGAlignCenter);
    page->setStyle(SkPaint::kFill_Style);
    page->setBackgroundColor(SK_ColorTRANSPARENT);
    if (blackWhiteMode) {
        page->setBlackWhiteMode();
    }
    blackWhiteMode = !blackWhiteMode;
    return page;
}

View *PageTest::getRootView() {
    auto page = context->getPageStackManager()->back();
    SkASSERT(page != nullptr && page->children.size() == 1);
    return page->children[0];
}
