#include "FilamentPage.h"
#include "FlexboxLayout.h"
#include "FilamentView.h"
#include "TextView.h"

namespace HYSkiaUI {

void FilamentPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void FilamentPage::initChildren(ViewGroup *root, int width, int height) {

    auto flexboxLayout = new FlexboxLayout();
    flexboxLayout->setContext(this->context);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setFlex(1);
    this->addView(flexboxLayout);

    {
        auto filament = new FilamentView();
        filament->setContext(this->context);
        filament->setWidth(width);
        filament->setHeight(800);
        filament->setStyle(SkPaint::kStroke_Style);
        filament->setBackgroundColor(SK_ColorTRANSPARENT);
        filament->setStrokeWidth(0);
        filament->setMargin({0, 0, 0, 50});
        flexboxLayout->addView(filament);
    }

    {
        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setText("Google Filament Preview using SurfaceTexture! Other 3D engines can also be supported");
        textView->setWidth(1000);
        textView->setHeight(200);
        textView->setTextColor(SK_ColorGREEN);
        textView->setTextSize(50);
        textView->setBackgroundColor(SK_ColorRED);
        textView->setStyle(SkPaint::kStroke_Style);
        textView->setMargin({50, 50, 50, 50});
        textView->setMaxLines(3);
        flexboxLayout->addView(textView);
    }
}

}
