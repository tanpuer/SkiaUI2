#include "QQMusicTest.h"
#include "PageStackManager.h"
#include "Button.h"
#include "ClockView.h"
#include "ProgressBar.h"
#include "LyricView.h"
#include "sstream"
#include "regex"
#include "random"
#include "AudioFFTView.h"

void QQMusicTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        auto page = initPage(width, height);
        root = page;
        testLyric(drawCount, root, width, height);
        context->getPageStackManager()->push(page);
        page->enterFromRight(Page::EnterExitInfo(width, 0));
    }
    calculate();
    for (const auto &item: context->getPageStackManager()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->measure();
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}

Page *QQMusicTest::initPage(int width, int height) {
    auto page = new Page();
    config = YGConfigNew();
    context->setConfigRef(config);
    page->setContext(context);
    page->setWidth(width);
    page->setHeight(height);
    page->setStyle(SkPaint::kFill_Style);
    page->setBackgroundColor(SK_ColorTRANSPARENT);
    return page;
}

View *QQMusicTest::getRootView() {
    auto page = context->getPageStackManager()->back();
    SkASSERT(page != nullptr && page->children.size() == 1);
    return page->children[0];
}

void QQMusicTest::testLyric(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    flexboxLayout->setContext(this->context);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setAlignItems(YGAlignCenter);
//    flexboxLayout->setJustifyContent(YGJustifyCenter);
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

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

void QQMusicTest::calculate() {
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
