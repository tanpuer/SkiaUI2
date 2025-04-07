#include "CameraPage.h"
#include "ScrollView.h"
#include "ProgressBar.h"
#include "Button.h"

namespace HYSkiaUI {

void CameraPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void CameraPage::initChildren(ViewGroup *root, int width, int height) {
    auto flexboxLayout = new ScrollView();
    flexboxLayout->setContext(this->context);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setFlex(1);
    this->addView(flexboxLayout);

    auto progressBar = new ProgressBar();
    progressBar->setContext(this->context);
    progressBar->setBackgroundColor(SK_ColorGRAY);
    progressBar->setBarColor(SK_ColorGREEN);
    progressBar->setStrokeWidth(10.0);
    progressBar->setAutoMode(false);
    progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
    progressBar->setProgress(50);
    progressBar->setStyle(SkPaint::kStroke_Style);
    progressBar->setWidth(width);
    progressBar->setHeight(60);
    progressBar->setMargin({50, 50, 50, 50});
    flexboxLayout->addView(progressBar);
    progressBar->setProgressCallback([this](int progress, bool finished) {
        auto height = progress * this->cameraView->getWidth() / 50;
        cameraView->setHeight(height);
    });

    auto rotateProgressBar = new ProgressBar();
    rotateProgressBar->setContext(this->context);
    rotateProgressBar->setBackgroundColor(SK_ColorGRAY);
    rotateProgressBar->setBarColor(SK_ColorGREEN);
    rotateProgressBar->setStrokeWidth(10.0);
    rotateProgressBar->setAutoMode(false);
    rotateProgressBar->setType(ProgressBar::ProgressBarType::LINEAR);
    rotateProgressBar->setProgress(0);
    rotateProgressBar->setStyle(SkPaint::kStroke_Style);
    rotateProgressBar->setWidth(width);
    rotateProgressBar->setHeight(60);
    rotateProgressBar->setMargin({50, 50, 50, 50});
    flexboxLayout->addView(rotateProgressBar);
    rotateProgressBar->setProgressCallback([this](int progress, bool finished) {
        auto rotateZ = progress / 100.0f * 360.0f;
        cameraView->setRotateZ(rotateZ);
    });

    {
        cameraView = new CameraView();
        cameraView->setContext(this->context);
        cameraView->setWidth(width);
        cameraView->setHeight(width);
        flexboxLayout->addView(cameraView);
    }
    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Switch Front/End Camera"));
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        button->setMargin({0, 50, 0, 0});
        flexboxLayout->addView(button);
        button->setOnClickListener([this](View *view) {
            cameraView->switchCamera();
        });
    }
}

}
