#include "CameraPage.h"
#include "ScrollView.h"
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
    setBackgroundColor(SK_ColorTRANSPARENT);
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
        button->setText(SkString("Capture"));
        button->setWidth(540);
        button->setHeight(100);
        button->setTextSize(60);
        button->setCornerRadius(20);
        button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
        button->setMargin({50, 0, 50, 100});
        flexboxLayout->addView(button);
        button->setOnClickListener([this](View *view) {
            ALOGD("setOnClickListener perform %s", view->name())
            this->cameraView->capture([this](sk_sp<SkImage> image){
                if (previewImageView != nullptr) {
                    previewImageView->setSkImage(image);
                    previewImageView->setWidth(image->width() / 4);
                    previewImageView->setHeight(image->height() / 4);
                    previewImageView->rotateZ = 90;
                }
            });
        });
    }
    {
        previewImageView = new ImageView();
        previewImageView->setContext(this->context);
        previewImageView->setScaleType(ImageView::ScaleType::FitCenter);
        previewImageView->setStyle(SkPaint::kStroke_Style);
        previewImageView->setBackgroundColor(SK_ColorRED);
        previewImageView->setStrokeWidth(2);
        flexboxLayout->addView(previewImageView);
    }
}

}
