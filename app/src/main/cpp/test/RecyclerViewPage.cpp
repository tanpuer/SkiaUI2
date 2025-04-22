#include "RecyclerViewPage.h"
#include "Button.h"

namespace HYSkiaUI {

void RecyclerViewPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void RecyclerViewPage::initChildren(HYSkiaUI::ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    flexboxLayout->setContext(this->context);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setFlex(1);
    root->addView(flexboxLayout);

    auto recyclerView = new MyRecyclerView();
    recyclerView->setContext(context);
    recyclerView->setDataSize(100000);
    recyclerView->setWidth(width);
    recyclerView->setHeight(height);
    flexboxLayout->addView(recyclerView);

    auto buttonContainer = new FlexboxLayout();
    buttonContainer->setContext(this->context);
    buttonContainer->setFlexWrap(YGWrapWrap);
    buttonContainer->setFlexDirection(YGFlexDirectionColumn);
    buttonContainer->setStyle(SkPaint::kFill_Style);
    buttonContainer->setBackgroundColor(SK_ColorTRANSPARENT);
    buttonContainer->setAlignSelf(YGAlign::YGAlignFlexEnd);
    buttonContainer->setPositionType(YGPositionType::YGPositionTypeAbsolute);
    buttonContainer->setAlignItems(YGAlignCenter);
    flexboxLayout->addView(buttonContainer);

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Position: 0"));
        button->setTextSize(30);
        button->setCornerRadius(20);
        button->setMargin({10, 0, 10, 10});
        buttonContainer->addView(button);
        button->setOnClickListener([recyclerView](View *view) {
            recyclerView->scrollToPosition(0);
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("smooth: 0"));
        button->setTextSize(30);
        button->setCornerRadius(20);
        button->setMargin({10, 0, 10, 10});
        buttonContainer->addView(button);
        button->setOnClickListener([recyclerView](View *view) {
            recyclerView->smoothScrollToPosition(0);
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Position: 9"));
        button->setTextSize(30);
        button->setCornerRadius(20);
        button->setMargin({10, 0, 10, 10});
        buttonContainer->addView(button);
        button->setOnClickListener([recyclerView](View *view) {
            recyclerView->scrollToPosition(9);
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("smooth: 9"));
        button->setTextSize(30);
        button->setCornerRadius(20);
        button->setMargin({10, 0, 10, 10});
        buttonContainer->addView(button);
        button->setOnClickListener([recyclerView](View *view) {
            recyclerView->smoothScrollToPosition(9);
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("Position: 99"));
        button->setTextSize(30);
        button->setCornerRadius(20);
        button->setMargin({10, 0, 10, 10});
        buttonContainer->addView(button);
        button->setOnClickListener([recyclerView](View *view) {
            recyclerView->scrollToPosition(99);
        });
    }

    {
        auto button = new Button();
        button->setContext(this->context);
        button->setText(SkString("smooth: 99"));
        button->setTextSize(30);
        button->setCornerRadius(20);
        button->setMargin({10, 0, 10, 10});
        buttonContainer->addView(button);
        button->setOnClickListener([recyclerView](View *view) {
            recyclerView->smoothScrollToPosition(99);
        });
    }
}

}
