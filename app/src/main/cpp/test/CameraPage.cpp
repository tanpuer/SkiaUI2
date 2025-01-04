#include "CameraPage.h"
#include "FlexboxLayout.h"
#include "CameraView.h"
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
        auto camera = new CameraView();
        camera->setContext(this->context);
        camera->setWidth(width);
        camera->setHeight(width);
        flexboxLayout->addView(camera);
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
        button->setOnClickListener([](View *view) {
            ALOGD("setOnClickListener perform %s", view->name())
        });
    }
}

}
