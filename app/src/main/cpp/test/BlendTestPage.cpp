#include "BlendTestPage.h"
#include "ScrollView.h"
#include "BlendTestView.h"
#include "TextView.h"
#include "ImageView.h"

namespace HYSkiaUI {

void BlendTestPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
    setContext(context);
    setWidth(width);
    setHeight(height);
    setFlexWrap(YGWrapWrap);
    setFlexDirection(YGFlexDirectionColumn);
    setJustifyContent(YGJustifyCenter);
    setAlignItems(YGAlignCenter);
    setAlignContent(YGAlignCenter);
    setStyle(SkPaint::kFill_Style);
    initChildren(this, width, height);
}

void BlendTestPage::initChildren(ViewGroup *root, int width, int height) {
    auto scrollView = new ScrollView();
    scrollView->setContext(this->context);
    scrollView->setFlexDirection(YGFlexDirectionColumn);
    scrollView->setStyle(SkPaint::kFill_Style);
    scrollView->setBackgroundColor(SK_ColorWHITE);
    scrollView->setAlignItems(YGAlignCenter);
    scrollView->setFlex(1);
    this->addView(scrollView);

    {
        auto flexboxLayout = new BlendTestPage::LightenFlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(width);
        flexboxLayout->setStyle(SkPaint::kFill_Style);
        flexboxLayout->setBackgroundColor(SK_ColorGRAY);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 0, 50});
        flexboxLayout->setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
        flexboxLayout->setJustifyContent(YGJustify::YGJustifyFlexStart);
        flexboxLayout->setAlignItems(YGAlign::YGAlignCenter);
        scrollView->addView(flexboxLayout);

        {
            auto imageView = new ImageView();
            imageView->setContext(this->context);
            imageView->setSource("bird.gif");
            imageView->setScaleType(ImageView::ScaleType::FitCenter);
            imageView->setStyle(SkPaint::kStroke_Style);
            imageView->setBackgroundColor(SK_ColorTRANSPARENT);
            imageView->setStrokeWidth(0);
            imageView->setWidth(400);
            imageView->setHeight(250);
            imageView->setScaleEffect(true);
            flexboxLayout->addView(imageView);
        }

        {
            auto imageView = new ImageView();
            imageView->setContext(this->context);
            imageView->setWidth(170 * 2);
            imageView->setHeight(46 * 2);
            imageView->setSource("music/logo.png");
            imageView->setScaleType(ImageView::ScaleType::CenterCrop);
            imageView->setStyle(SkPaint::kStroke_Style);
            imageView->setBackgroundColor(SK_ColorTRANSPARENT);
            imageView->setStrokeWidth(0);
            flexboxLayout->addView(imageView);
        }

        {
            auto textView = new TextView();
            textView->setContext(this->context);
            textView->setText("羊肉烤串");
            textView->setTextColor(SK_ColorGREEN);
            textView->setTextSize(50);
            textView->setBackgroundColor(SK_ColorTRANSPARENT);
            textView->setStyle(SkPaint::kStroke_Style);
            textView->setMargin({50, 50, 50, 50});
            flexboxLayout->addView(textView);
        }
    }

    static std::unordered_map<std::string, SkBlendMode> blendMap = {
            {"kSrc",        SkBlendMode::kSrc},
            {"kDst",        SkBlendMode::kDst},
            {"kSrcOver",    SkBlendMode::kSrcOver},
            {"kDstOver",    SkBlendMode::kDstOver},
            {"kSrcIn",      SkBlendMode::kSrcIn},
            {"kDstIn",      SkBlendMode::kDstIn},
            {"kSrcOut",     SkBlendMode::kSrcOut},
            {"kDstOut",     SkBlendMode::kDstOut},
            {"kSrcATop",    SkBlendMode::kSrcATop},
            {"kDstATop",    SkBlendMode::kDstATop},
            {"kXor",        SkBlendMode::kXor},
            {"kPlus",       SkBlendMode::kPlus},
            {"kModulate",   SkBlendMode::kModulate},
            {"kScreen",     SkBlendMode::kScreen},
            {"kOverlay",    SkBlendMode::kOverlay},
            {"kDarken",     SkBlendMode::kDarken},
            {"kLighten",    SkBlendMode::kLighten},
            {"kColorDodge", SkBlendMode::kColorDodge},
            {"kColorBurn",  SkBlendMode::kColorBurn},
            {"kHardLight",  SkBlendMode::kHardLight},
            {"kSoftLight",  SkBlendMode::kSoftLight},
            {"kDifference", SkBlendMode::kDifference},
            {"kExclusion",  SkBlendMode::kExclusion},
            {"kMultiply",   SkBlendMode::kMultiply},
            {"kHue",        SkBlendMode::kHue},
            {"kSaturation", SkBlendMode::kSaturation},
            {"kColor",      SkBlendMode::kColor},
            {"kLuminosity", SkBlendMode::kLuminosity},
    };

    for (auto &item: blendMap) {
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(width);
        flexboxLayout->setHeight(600);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorWHITE);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 0, 50});
        flexboxLayout->setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
        flexboxLayout->setJustifyContent(YGJustify::YGJustifyFlexStart);
        flexboxLayout->setAlignItems(YGAlign::YGAlignCenter);
        scrollView->addView(flexboxLayout);

        auto canvasTest = new BlendTestView();
        canvasTest->setContext(this->context);
        canvasTest->setBlendMode(item.second);
        canvasTest->setWidth(600);
        canvasTest->setHeight(600);
        canvasTest->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(canvasTest);

        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setText(item.first.c_str());
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(50);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        textView->setMargin({50, 50, 50, 50});
        flexboxLayout->addView(textView);
    }

}

}
