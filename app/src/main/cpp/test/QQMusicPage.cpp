#include "QQMusicPage.h"
#include "FlexboxLayout.h"
#include "ImageView.h"
#include "LyricView.h"
#include "LyricScrollView.h"
#include "LinearAnimator.h"
#include "SVGView.h"

namespace HYSkiaUI {

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
    flexboxLayout->setFlex(1);
    this->addView(flexboxLayout);

    {
        auto imageView = new ImageView();
        imageView->setContext(this->context);
        imageView->setWidthPercent(100);
        imageView->setHeightPercent(100);
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
        auto svgView = new SVGView();
        svgView->setContext(this->context);
        svgView->setSource("music/mv_definition_super.svg");
        svgView->setStyle(SkPaint::kStroke_Style);
        svgView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        svgView->setBackgroundColor(SK_ColorTRANSPARENT);
        svgView->setWidth(100);
        svgView->setHeight(100);
        svgView->setAlignSelf(YGAlignFlexStart);
        svgView->setMargin({50, 200, 0, 0});
        flexboxLayout->addView(svgView);
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
        recordOutAnimator = new LinearAnimator(imageView, 0.0, 360.0);
        recordOutAnimator->setDuration(5000);
        recordOutAnimator->setLoopCount(-1);
        recordOutAnimator->setUpdateListener([imageView](View *view, float value) {
            imageView->rotateZ = value;
        });
        recordOutAnimator->start();
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
        recordInnerAnimator = new LinearAnimator(imageView, 0.0, 360.0);
        recordInnerAnimator->setDuration(5000);
        recordInnerAnimator->setLoopCount(-1);
        recordInnerAnimator->setUpdateListener([imageView](View *view, float value) {
            imageView->rotateZ = value;
        });
        recordInnerAnimator->start();
    }

    {
        armView = new ImageView();
        armView->setContext(this->context);
        armView->setWidth(130 * 1.5);
        armView->setHeight(600 * 1.5);
        armView->setSource("music/record_player_dark_arm.png");
        armView->setScaleType(ImageView::ScaleType::CenterCrop);
        armView->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        armView->setAlignSelf(YGAlignFlexStart);
        armView->setStyle(SkPaint::kStroke_Style);
        armView->setBackgroundColor(SK_ColorTRANSPARENT);
        armView->setMargin({700, 350, 0, 0});
        armView->needClip = false;
        flexboxLayout->addView(armView);
        armView->setRotateFunc([](SkRect &dstRect, SkMatrix &matrix, float rotateZ) {
            matrix.preRotate(rotateZ, dstRect.centerX(), dstRect.top());
        });
        updateArmView(true);
    }

//    auto lyricView = new LyricView();
//    lyricView->setContext(this->context);
//    lyricView->setBackgroundColor("#00000000");
//    lyricView->setWidth(width);
//    lyricView->setHeight(height - 700);
//    lyricView->setSourceSRT("feng.srt");
//    lyricView->setMargin({0, 300, 0, 0});
//    flexboxLayout->addView(lyricView);

    lyricView = new LyricScrollView();
    lyricView->setContext(this->context);
    lyricView->setBackgroundColor("#00000000");
    lyricView->setWidthPercent(100);
    lyricView->setFlex(3);
    lyricView->setSourceSRT("feng.srt");
    lyricView->setMargin({0, 300, 0, 0});
    flexboxLayout->addView(lyricView);

    fftView = new AudioFFTView();
    fftView->setContext(this->context);
    fftView->setWidthPercent(100);
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
            imageView->setOnClickListener([this](View *view) {
                fftView->seek(0);
                playImage->setSource("music/ic_pause.png");
            });
            controlView->addView(imageView);
        }
        {
            playImage = new ImageView();
            playImage->setContext(this->context);
            playImage->setWidth(72 * 1.5);
            playImage->setHeight(72 * 1.5);
            playImage->setSource("music/ic_pause.png");
            playImage->setScaleType(ImageView::ScaleType::CenterCrop);
            playImage->setAlignSelf(YGAlignFlexStart);
            playImage->setStyle(SkPaint::kStroke_Style);
            playImage->setBackgroundColor(SK_ColorTRANSPARENT);
            playImage->setMargin({100, 0, 0, 0});
            playImage->setOnClickListener([this](View *view) {
                if (fftView != nullptr) {
                    if (fftView->isPlaying()) {
                        playImage->setSource("music/ic_play.png");
                        fftView->pause();
                        this->updateArmView(false);
                    } else {
                        playImage->setSource("music/ic_pause.png");
                        fftView->play();
                        this->updateArmView(true);
                    }
                }
            });
            controlView->addView(playImage);
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
            imageView->setOnClickListener([this](View *view) {
                if (fftView != nullptr) {
                    fftView->seek(0);
                    playImage->setSource("music/ic_pause.png");
                }
            });
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
        progressBar->setWidthPercent(100);
        progressBar->setHeight(60);
        progressBar->setMargin({50, 150, 50, 50});
        progressBar->setPositionType(YGPositionType::YGPositionTypeAbsolute);
        flexboxLayout->addView(progressBar);
        progressBar->setProgressCallback([this](int progress, bool finished) {
            ALOGD("ProgressBar progress: %d", progress)
            if (finished && fftView != nullptr) {
                fftView->seek(progress * fftView->getDuration() / 100);
                playImage->setSource("music/ic_pause.png");
                updateArmView(true);
            }
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

void QQMusicPage::updateArmView(bool play) {
    if (armView != nullptr) {
        auto value = play ? 30.0 : 0.0;
        auto rotateAnimator = new LinearAnimator(armView, armView->rotateZ, value);
        rotateAnimator->setDuration(500);
        rotateAnimator->setUpdateListener([this](View *view, float value) {
            armView->rotateZ = value;
        });
        rotateAnimator->start();
    }
    if (recordOutAnimator != nullptr) {
        if (play) {
            recordOutAnimator->resume();
        } else {
            recordOutAnimator->pause();
        }
    }
    if (recordInnerAnimator != nullptr) {
        if (play) {
            recordInnerAnimator->resume();
        } else {
            recordInnerAnimator->pause();
        }
    }
}

}