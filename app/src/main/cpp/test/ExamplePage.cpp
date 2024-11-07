#include "ExamplePage.h"
#include "ScrollView.h"
#include "VideoView.h"
#include "LottieView.h"
#include "ShaderView.h"
#include "ImageView.h"
#include "SVGView.h"
#include "CanvasTest.h"
#include "ClockView.h"
#include "ProgressBar.h"
#include "MovingView.h"
#include "Button.h"
#include "TextView.h"
#include "QQMusicPage.h"
#include "YUVVideoView.h"
#include "LoadingView.h"
#include "Icon.h"
#include "Radio.h"
#include "Switch.h"
#include "RichText.h"
#include "PickerView.h"

void ExamplePage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
    setContext(context);
    setWidth(width);
    setHeight(height);
    setFlexWrap(YGWrapWrap);
    setFlexDirection(YGFlexDirectionColumn);
    setJustifyContent(YGJustifyCenter);
    setAlignItems(YGAlignCenter);
    setAlignContent(YGAlignCenter);
    setStyle(SkPaint::kFill_Style);
    setBackgroundColor(SK_ColorTRANSPARENT);
    if (blackWhiteMode) {
        setBlackWhiteMode();
    }
    blackWhiteMode = !blackWhiteMode;

    initChildren(this, width, height);
}

void ExamplePage::drawOnFrame(int drawCount) {
    Page::drawOnFrame(drawCount);
}

void ExamplePage::initChildren(ViewGroup *root, int width, int height) {
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
        auto richText = new RichText();
        richText->setContext(this->context);
        richText->setWidth(width);
        richText->setStyle(SkPaint::kStroke_Style);
        richText->setBackgroundColor(SK_ColorTRANSPARENT);
        richText->setStrokeWidth(0);
        this->setOnPageSizeChangeListener([richText](int width, int height) {
            richText->setWidth(width);
        });
        scrollView->addView(richText);
    }

    {
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(1080);
        flexboxLayout->setHeight(360 * 1080 / 640);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorTRANSPARENT);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 0, 50});
        scrollView->addView(flexboxLayout);

        auto videoView = new YUVVideoView();
        videoView->setContext(this->context);
        videoView->setWidthPercent(100);
        videoView->setHeightPercent(100);
        videoView->setSource("yiluxiangbei.mp4");
        videoView->setStyle(SkPaint::kStroke_Style);
        videoView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        flexboxLayout->addView(videoView);

        auto loadingView = new LoadingView();
        loadingView->setContext(this->context);
        loadingView->setWidthPercent(100);
        loadingView->setHeightPercent(100);
        loadingView->setStyle(SkPaint::kStroke_Style);
        loadingView->setBackgroundColor(SK_ColorTRANSPARENT);
        loadingView->setStrokeWidth(0);
        loadingView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        flexboxLayout->addView(loadingView);

        videoView->setRenderFirstFrameCallback([loadingView, flexboxLayout]() {
            flexboxLayout->removeView(loadingView);
        });
    }

    {
        auto lottieView = new LottieView();
        lottieView->setContext(this->context);
        lottieView->setWidth(375);
        lottieView->setHeight(240);
        lottieView->setSource("WorkspacePlanet.json");
        lottieView->setStyle(SkPaint::kStroke_Style);
        lottieView->setBackgroundColor(SK_ColorRED);
        lottieView->setStrokeWidth(2);
        lottieView->setMargin({0, 0, 0, 50});
        scrollView->addView(lottieView);
        lottieView->setOnClickListener([this, width, height](View *view) {
            auto page = new QQMusicPage();
            page->init(context, width, height);
            context->getPageStackManager()->push(page);
            page->enterFromRight(Page::EnterExitInfo(width, 0));
        });
    }

    {
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(1080);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorTRANSPARENT);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 0, 50});
        flexboxLayout->setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
        flexboxLayout->setJustifyContent(YGJustify::YGJustifyCenter);
        flexboxLayout->setAlignItems(YGAlign::YGAlignCenter);
        scrollView->addView(flexboxLayout);

        {
            auto view = new View();
            view->setContext(this->context);
            auto colors = std::vector<SkColor>();
            colors.push_back(SK_ColorYELLOW);
            colors.push_back(SK_ColorBLUE);
            view->setLinearGradient(colors);
            view->setWidth(200);
            view->setHeight(200);
            flexboxLayout->addView(view);
            view->setOnClickListener([this, width, height](View *view) {
                MeasureTime measureTime("pushPage");
                auto page = new ExamplePage();
                page->init(context, width, height);
                context->getPageStackManager()->push(page);
                page->enterFromRight(Page::EnterExitInfo(width, 0));
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
            view->setMargin({200, 50, 0, 0});
            flexboxLayout->addView(view);
            view->setOnClickListener([this, width, height](View *view) {
                if (context->getPageStackManager()->getPages().size() <= 1) {
                    context->getPluginManager()->invokeMethod("toast", "show", "back error");
                    return;
                }
                auto page = context->getPageStackManager()->back();
                if (page == nullptr) {
                    ALOGE("pop failed due to empty pages")
                    return;
                }
                page->exitToLeft(Page::EnterExitInfo(0, width));
//            page->exitToTop(Page::EnterExitInfo(0, height));
            });
        }
    }

    {
        auto shaderView = new ShaderView();
        shaderView->setContext(this->context);
        shaderView->setShaderPath("raining.glsl", {"raining.png"});
        shaderView->setWidth(1080);
        shaderView->setHeight(520);
        scrollView->addView(shaderView);
        shaderView->setOnClickListener([shaderView](View *view) -> void {
            static bool flag = false;
            if (flag) {
                shaderView->setShaderPath("raining.glsl", {"raining.png"});
            } else {
                shaderView->setShaderPath("sincos.glsl");
            }
            flag = !flag;
        });
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
        imageView->setOnCompleteFunc([](ImageView *imageView) {
            static bool flag = true;
            imageView->blur(flag ? 10.0f : 0.0f);
            flag = !flag;
        });
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
        progressBar->setProgressCallback([](int progress, bool finished) {
            ALOGD("ProgressBar progress: %d %d", progress, finished)
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
        textView->setText(SkString(
                "😀😃😄😁😆😅😂🤣☺😇🙂😍😡😟😢😻👽💩👍👎🙏👌👋👄👁👦👼👨‍🚀👨‍🚒🙋‍♂️👳👨‍👨‍👧"
                "👧💼👡👠☂🐶🐰🐻🐼🐷🐒🐵🐔🐧🐦🐋🐟🐡🕸🐌🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝"
                "💦💧❄\n🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏"
                "🔑📁🗓📊\n❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmojiShow"));
        textView->setWidth(1000);
        textView->setHeight(200);
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(50);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        textView->setMargin({50, 50, 50, 50});
        textView->setMaxLines(3);
        textView->setEllipsis("点击展开");
        textView->setOnClickListener([textView](View *view) -> void {
            static bool flag = true;
            textView->setMaxLines(flag ? 0 : 3);
            flag = !flag;
        });
        scrollView->addView(textView);
    }

    {
        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(60);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        auto paint1 = SkPaint();
        paint1.setAntiAlias(true);
        paint1.setColor(SK_ColorCYAN);
        textView->pushText(TextView::StringBuilder(SkString("这是"),
                                                   SkFontStyle(SkFontStyle::kThin_Weight,
                                                               SkFontStyle::kNormal_Width,
                                                               SkFontStyle::kUpright_Slant),
                                                   40, paint1));
        auto paint2 = SkPaint();
        paint2.setAntiAlias(true);
        paint2.setColor(SK_ColorGREEN);
        textView->pushText(TextView::StringBuilder(SkString("阿里妈妈方圆体"),
                                                   SkFontStyle(SkFontStyle::kThin_Weight,
                                                               SkFontStyle::kNormal_Width,
                                                               SkFontStyle::kUpright_Slant),
                                                   100, paint2));
        auto paint4 = SkPaint();
        paint4.setAntiAlias(true);
        paint4.setColor(SK_ColorRED);
        textView->pushText(TextView::StringBuilder(SkString("demo"),
                                                   SkFontStyle(SkFontStyle::kThin_Weight,
                                                               SkFontStyle::kNormal_Width,
                                                               SkFontStyle::kUpright_Slant),
                                                   100, paint4));
        textView->setMargin({50, 50, 50, 50});
        scrollView->addView(textView);
    }

    {
        auto loadingView = new LoadingView();
        loadingView->setContext(this->context);
        loadingView->setWidth(1080);
        loadingView->setHeight(200);
        loadingView->setMargin({0, 0, 0, 50});
        loadingView->setStyle(SkPaint::kStroke_Style);
        scrollView->addView(loadingView);
    }

    {
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(980);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorTRANSPARENT);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({50, 0, 50, 50});
        flexboxLayout->setAlignItems(YGAlign::YGAlignCenter);
        flexboxLayout->setJustifyContent(YGJustifySpaceBetween);
        flexboxLayout->setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
        scrollView->addView(flexboxLayout);

        std::unordered_map<int32_t, SkColor> iconInfos{
                {0xe615, SK_ColorRED},
                {0xe7ce, SK_ColorGREEN},
                {0xe670, SkColorSetARGB(255, 31, 132, 226)},
                {0xe67d, SK_ColorGREEN},
                {0xe606, SK_ColorGREEN},
                {0xe6a2, SK_ColorGREEN},
                {0xe61f, SK_ColorBLUE},

        };
        for (auto &info: iconInfos) {
            auto icon = new Icon();
            icon->setContext(this->context);
            icon->setIcon(info.first);
            icon->setStyle(SkPaint::kStroke_Style);
            icon->setStrokeWidth(1);
            icon->setIconSize(100);
            icon->setIconColor(info.second);
            flexboxLayout->addView(icon);
        }
    }

    {
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(980);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorTRANSPARENT);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 50, 50});
        scrollView->addView(flexboxLayout);
        std::unordered_map<std::string, bool> frameworks{
                {"React-Native", true},
                {"Appx",         true},
                {"Simplex",      false}
        };
        for (auto &value: frameworks) {
            auto group = new FlexboxLayout();
            group->setContext(this->context);
            group->setStyle(SkPaint::kStroke_Style);
            group->setBackgroundColor(SK_ColorTRANSPARENT);
            group->setStrokeWidth(0);
            group->setMargin({0, 0, 50, 50});
            group->setAlignItems(YGAlign::YGAlignCenter);
            group->setFlexDirection(YGFlexDirectionRow);
            flexboxLayout->addView(group);
            group->setOnClickListener([](View *view) -> void {
                auto radio = reinterpret_cast<Radio *>(reinterpret_cast<FlexboxLayout *>(view)->children[0]);
                if (radio != nullptr) {
                    radio->getClickListener()(radio);
                }
            });
            auto radio = new Radio();
            radio->setContext(this->context);
            radio->setSelected(value.second);
            radio->setWidth(100);
            radio->setHeight(100);
            group->addView(radio);
            auto label = new TextView();
            label->setContext(this->context);
            label->setText(value.first.c_str());
            label->setMargin({50, 0, 0, 0});
            label->setTextSize(50);
            label->setTextColor(SK_ColorBLACK);
            label->setOnClickListener([radio](View *view) -> void {
                radio->getClickListener()(radio);
            });
            group->addView(label);
        }

    }

    {

        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(980);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorTRANSPARENT);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 50, 50});
        scrollView->addView(flexboxLayout);

        {
            auto switchView = new Switch();
            switchView->setContext(this->context);
            switchView->setMargin({0, 0, 0, 50});
            flexboxLayout->addView(switchView);
        }
        {
            auto switchView = new Switch();
            switchView->setContext(this->context);
            switchView->setColor(SK_ColorRED);
            switchView->setEnabled(false);
            flexboxLayout->addView(switchView);
        }
    }

    {
        auto pickerView = new PickerView();
        pickerView->setContext(this->context);
        pickerView->setWidth(width);
        for (int i = 0; i < 17; ++i) {
            auto fbLayout = new FlexboxLayout();
            fbLayout->setContext(this->context);
            fbLayout->setAlignItems(YGAlign::YGAlignCenter);
            fbLayout->setJustifyContent(YGJustify::YGJustifyCenter);
            fbLayout->setWidthPercent(100);
            fbLayout->setHeight(120);
            pickerView->addView(fbLayout);

            auto value = 2008 + i;
            auto textView = new TextView();
            textView->setContext(this->context);
            textView->setText(std::to_string(value).c_str());
            textView->setTextColor(SK_ColorBLACK);
            textView->setTextSize(50);
            if (i == 0) {
                fbLayout->setMargin({0, 120 * 3, 0, 0});
            } else if (i == 16) {
                fbLayout->setMargin({0, 0, 0, 120 * 3});
            }
            textView->setTextAlign(skia::textlayout::TextAlign::kCenter);
            fbLayout->addView(textView);
        }
        scrollView->addView(pickerView);
    }
}
