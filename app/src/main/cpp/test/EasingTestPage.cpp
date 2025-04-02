#include "EasingTestPage.h"
#include "ScrollView.h"
#include "LottieView.h"
#include "LinearAnimator.h"
#include "TextView.h"

namespace HYSkiaUI {

void EasingTestPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void EasingTestPage::initChildren(ViewGroup *root, int width, int height) {
    auto scrollView = new ScrollView();
    scrollView->setContext(this->context);
    scrollView->setFlexDirection(YGFlexDirectionColumn);
    scrollView->setStyle(SkPaint::kFill_Style);
    scrollView->setBackgroundColor(SK_ColorWHITE);
    scrollView->setAlignItems(YGAlignCenter);
    scrollView->setFlex(1);
    this->addView(scrollView);

    static std::unordered_map<std::string, EaseType> sEasingMap = {
            {
                    {"linear", EaseType::Linear},
                    {"SineIn", EaseType::SineIn},
                    {"SineOut", EaseType::SineOut},
                    {"SineInOut", EaseType::SineInOut},
                    {"QuadIn", EaseType::QuadIn},
                    {"QuadOut", EaseType::QuadOut},
                    {"QuadInOut", EaseType::QuadInOut},
                    {"CubicIn", EaseType::CubicIn},
                    {"CubicOut", EaseType::CubicOut},
                    {"CubicInOut", EaseType::CubicInOut},
                    {"QuartIn", EaseType::QuartIn},
                    {"QuartOut", EaseType::QuartOut},
                    {"QuartInOut", EaseType::QuartInOut},
                    {"QuintIn", EaseType::QuintIn},
                    {"QuintOut", EaseType::QuintOut},
                    {"QuintInOut", EaseType::QuintInOut},
                    {"ExpoIn", EaseType::ExpoIn},
                    {"ExpoOut", EaseType::ExpoOut},
                    {"ExpoInOut", EaseType::ExpoInOut},
                    {"CircIn", EaseType::CircIn},
                    {"CircOut", EaseType::CircOut},
                    {"CircInOut", EaseType::CircInOut},
                    {"BackIn", EaseType::BackIn},
                    {"BackOut", EaseType::BackOut},
                    {"BackInOut", EaseType::BackInOut},
                    {"ElasticIn", EaseType::ElasticIn},
                    {"ElasticOut", EaseType::ElasticOut},
                    {"ElasticInOut", EaseType::ElasticInOut},
                    {"BounceIn", EaseType::BounceIn},
                    {"BounceOut", EaseType::BounceOut},
                    {"BounceInOut", EaseType::BounceInOut},
            }
    };

    for (auto &item: sEasingMap) {
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(this->context);
        flexboxLayout->setWidth(width);
        flexboxLayout->setHeight(400);
        flexboxLayout->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->setBackgroundColor(SK_ColorWHITE);
        flexboxLayout->setStrokeWidth(0);
        flexboxLayout->setMargin({0, 0, 0, 50});
        flexboxLayout->setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
        flexboxLayout->setJustifyContent(YGJustify::YGJustifyFlexStart);
        flexboxLayout->setAlignItems(YGAlign::YGAlignCenter);
        scrollView->addView(flexboxLayout);

        auto lottieView = new LottieView();
        lottieView->setContext(this->context);
        lottieView->setWidth(375);
        lottieView->setHeight(240);
        lottieView->setSource("WorkspacePlanet.json");
        lottieView->setStyle(SkPaint::kStroke_Style);
        lottieView->setBackgroundColor(SK_ColorRED);
        lottieView->setStrokeWidth(2);
        flexboxLayout->addView(lottieView);
        auto lottieAnimator = new LinearAnimator(lottieView, 0.0f, 360.0f);
        lottieAnimator->setDuration(10000);
        lottieAnimator->setLoopCount(-1);
        lottieAnimator->setEaseType(item.second);
        lottieAnimator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(360.0f - value);
        });
        lottieAnimator->start();

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
