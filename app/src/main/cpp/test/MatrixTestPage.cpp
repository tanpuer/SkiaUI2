#include "MatrixTestPage.h"
#include "ScrollView.h"
#include "LinearAnimator.h"
#include "TextView.h"
#include "ImageView.h"
#include "ExoPlayerView.h"
#include "LottieView.h"
#include "Button.h"

namespace HYSkiaUI {


void MatrixTestPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void MatrixTestPage::initChildren(ViewGroup *root, int width, int height) {
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
    scrollView->setWidth(width);
    scrollView->setHeight(height);
    root->addView(scrollView);

    {
        auto view = new View();
        view->setContext(this->context);
        view->setWidth(200);
        view->setHeight(200);
        view->setBackgroundColor(SK_ColorRED);
        scrollView->addView(view);
        auto animator = new LinearAnimator(view, 0.0, 360.0);
        animator->setDuration(10000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
        });
        animator->start();
    }

    {
        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setText(SkString("Rotate text test"));
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(50);
        textView->setBackgroundColor(SK_ColorTRANSPARENT);
        textView->setStyle(SkPaint::kStroke_Style);
        auto animator = new LinearAnimator(textView, 0.0, 360.0);
        animator->setDuration(10000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
            auto scale = 2.0f - abs(value - 180.0f) / 180.0f;
            view->setScaleX(scale);
            view->setScaleY(scale);
        });
        animator->start();
        scrollView->addView(textView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setSource("bird.gif");
        imageView->setScaleType(ImageView::ScaleType::FitCenter);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorRED);
        imageView->setStrokeWidth(2);
        imageView->setWidth(400);
        imageView->setHeight(400);
        imageView->setScaleEffect(true);
        scrollView->addView(imageView);
        auto animator = new LinearAnimator(imageView, 0.0, 360.0);
        animator->setDuration(10000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
        });
        animator->start();
    }

    {
        auto videoView = new ExoPlayerView();
        videoView->setContext(this->context);
        videoView->setWidth(1080);
        videoView->setHeight(360 * 1080 / 640);
        videoView->setSource("yiluxiangbei.mp4");
        videoView->setStyle(SkPaint::kStroke_Style);
        scrollView->addView(videoView);
        auto animator = new LinearAnimator(videoView, 0.0, 360.0);
        animator->setDuration(10000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
        });
        animator->start();
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
        scrollView->addView(lottieView);
        auto animator = new LinearAnimator(lottieView, 0.0, 360.0);
        animator->setDuration(10000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
        });
        animator->start();
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Button"));
        button->setWidth(540);
        button->setHeight(100);
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->setMargin({50, 50, 50, 50});
        scrollView->addView(button);
        auto animator = new LinearAnimator(button, 0.0, 360.0);
        animator->setDuration(10000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
        });
        animator->start();
    }
}

}
