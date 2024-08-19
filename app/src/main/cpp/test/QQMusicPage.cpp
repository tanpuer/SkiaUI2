#include "QQMusicPage.h"
#include "FlexboxLayout.h"
#include "ImageView.h"
#include "LyricView.h"

void QQMusicPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
    setContext(context);
    setWidth(width);
    setHeight(height);
    setStyle(SkPaint::kFill_Style);
    setBackgroundColor(SK_ColorTRANSPARENT);

    auto flexboxLayout = new FlexboxLayout();
    flexboxLayout->setContext(this->context);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    this->addView(flexboxLayout);

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setWidth(width);
        imageView->setHeight(height);
        imageView->setSource("music/bg.png");
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        imageView->blur(10.0f);
        flexboxLayout->addView(imageView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setWidth(170 * 2);
        imageView->setHeight(46 * 2);
        imageView->setSource("music/logo.png");
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        imageView->setAlignSelf(YGAlignFlexStart);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorTRANSPARENT);
        imageView->setStrokeWidth(1);
        flexboxLayout->addView(imageView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setWidth(480 * 2);
        imageView->setHeight(480 * 2);
        imageView->setSource("music/record_player_verydark_highlight.png");
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        imageView->setAlignSelf(YGAlignCenter);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorTRANSPARENT);
        imageView->setMargin({0, 300, 0, 0});
        flexboxLayout->addView(imageView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setWidth(160 * 2);
        imageView->setHeight(160 * 2);
        imageView->setCornerRadius(160);
        imageView->setSource("music/bg.png");
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        imageView->setAlignSelf(YGAlignCenter);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorTRANSPARENT);
        imageView->setMargin({0, 620, 0, 0});
        flexboxLayout->addView(imageView);
    }

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setWidth(130 * 1.5);
        imageView->setHeight(600 * 1.5);
        imageView->setSource("music/record_player_dark_arm.png");
        imageView->setScaleType(ImageView::ScaleType::CenterCrop);
        imageView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        imageView->setAlignSelf(YGAlignFlexStart);
        imageView->setStyle(SkPaint::kStroke_Style);
        imageView->setBackgroundColor(SK_ColorTRANSPARENT);
        imageView->setMargin({700, 350, 0, 0});
        flexboxLayout->addView(imageView);
    }

    auto lyricView = new LyricView();
    lyricView->setContext(this->context);
    lyricView->setBackgroundColor("#00000000");
    lyricView->setWidth(width);
    lyricView->setHeight(height - 700);
    lyricView->setSourceSRT("feng.srt");
    lyricView->setMargin({0, 300, 0, 0});
    flexboxLayout->addView(lyricView);

    fftView = new AudioFFTView();
    fftView->setContext(this->context);
    fftView->setWidth(width);
    fftView->setFlex(1);
    fftView->setBackgroundColor("#00000000");
    fftView->setSource("feng.mp4");
    flexboxLayout->addView(fftView);
    lyricView->setCurrPositionFunc([this]() -> long {
        return fftView->getCurrPosition();
    });

    {
        auto controlView = new FlexboxLayout();
        controlView->setContext(this->context);
        controlView->setFlexWrap(YGWrapWrap);
        controlView->setFlexDirection(YGFlexDirectionRow);
        controlView->setStyle(SkPaint::kFill_Style);
        controlView->setBackgroundColor(SK_ColorTRANSPARENT);
        controlView->setAlignSelf(YGAlignFlexEnd);
        controlView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        controlView->setMargin({400, 0, 0, 0});
        flexboxLayout->addView(controlView);
        {
            auto imageView = new ImageView();
            imageView->setContext(this->context);
            imageView->setWidth(72 * 1.5);
            imageView->setHeight(72 * 1.5);
            imageView->setSource("music/ic_previous.png");
            imageView->setScaleType(ImageView::ScaleType::CenterCrop);
            imageView->setAlignSelf(YGAlignFlexStart);
            imageView->setStyle(SkPaint::kStroke_Style);
            imageView->setBackgroundColor(SK_ColorTRANSPARENT);
            imageView->setOnClickListener([](View *view) {});
            controlView->addView(imageView);
        }
        {
            auto imageView = new ImageView();
            imageView->setContext(this->context);
            imageView->setWidth(72 * 1.5);
            imageView->setHeight(72 * 1.5);
            imageView->setSource("music/ic_play.png");
            imageView->setScaleType(ImageView::ScaleType::CenterCrop);
            imageView->setAlignSelf(YGAlignFlexStart);
            imageView->setStyle(SkPaint::kStroke_Style);
            imageView->setBackgroundColor(SK_ColorTRANSPARENT);
            imageView->setMargin({100, 0, 0, 0});
            imageView->setOnClickListener([](View *view) {});
            controlView->addView(imageView);
        }
        {
            auto imageView = new ImageView();
            imageView->setContext(this->context);
            imageView->setWidth(72 * 1.5);
            imageView->setHeight(72 * 1.5);
            imageView->setSource("music/ic_next.png");
            imageView->setScaleType(ImageView::ScaleType::CenterCrop);
            imageView->setAlignSelf(YGAlignFlexStart);
            imageView->setStyle(SkPaint::kStroke_Style);
            imageView->setBackgroundColor(SK_ColorTRANSPARENT);
            imageView->setMargin({100, 0, 100, 0});
            imageView->setOnClickListener([](View *view) {});
            controlView->addView(imageView);
        }
    }

    {
        progressBar = new ProgressBar();
        progressBar->setContext(this->context);
        progressBar->setBarColor(SK_ColorGREEN);
        progressBar->setBackgroundColor(SK_ColorGRAY);
        progressBar->setStrokeWidth(10.0);
        progressBar->setAutoMode(false);
        progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
        progressBar->setProgress(0);
        progressBar->setStyle(SkPaint::kStroke_Style);
        progressBar->setWidth(width);
        progressBar->setHeight(60);
        progressBar->setMargin({50, 150, 50, 50});
        progressBar->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        flexboxLayout->addView(progressBar);
        progressBar->setProgressCallback([](int progress) {
            ALOGD("ProgressBar progress: %d", progress)
        });
    }
}

void QQMusicPage::drawOnFrame(int drawCount) {
    if (fftView != nullptr && progressBar != nullptr) {
        auto duration = fftView->getDuration();
        auto current = fftView->getCurrPosition();
        if (duration == 0) {
            return;
        }
        progress = current * 100.0f / duration;
        progressBar->setProgress(progress);
    }
}
