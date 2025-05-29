#include "VideoShaderTest.h"
#include "ScrollView.h"
#include "ExoPlayerView.h"
#include "Button.h"

namespace HYSkiaUI {

void VideoShaderTest::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void VideoShaderTest::initChildren(ViewGroup *root, int width, int height) {
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

    auto videoView = new ExoPlayerView();
    videoView->setContext(this->context);
    videoView->setWidth(1080);
    videoView->setHeight(360 * 1080 / 640);
    videoView->setSource("yiluxiangbei.mp4");
    videoView->setRepeatMode(true);
    videoView->setStyle(SkPaint::kStroke_Style);
    scrollView->addView(videoView);

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("BlackWhite"));
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        button->setMargin({0, 20, 0, 0});
        scrollView->addView(button);
        button->setOnClickListener([videoView](View *view) {
            videoView->setShaderPath("skia_video_black_white.glsl");
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Lightning"));
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        button->setMargin({0, 20, 0, 0});
        scrollView->addView(button);
        button->setOnClickListener([videoView](View *view) {
            videoView->setShaderPath("skia_video_lightning_shader.glsl");
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Raining"));
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        button->setMargin({0, 20, 0, 0});
        scrollView->addView(button);
        button->setOnClickListener([videoView](View *view) {
            videoView->setShaderPath("skia_video_raining_shader.glsl");
        });
    }

}

}
