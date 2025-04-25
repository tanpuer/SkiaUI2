#include "GridLayoutTest.h"
#include "GridLayout.h"
#include "AndroidImageView.h"

namespace HYSkiaUI {

void GridLayoutTest::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void GridLayoutTest::initChildren(ViewGroup *root, int width, int height) {
    auto gridLayout = new GridLayout();
    gridLayout->setContext(this->context);
    gridLayout->setStyle(SkPaint::kFill_Style);
    gridLayout->setBackgroundColor(SK_ColorWHITE);
    gridLayout->setRowCount(4);
    gridLayout->setFlex(1);
    this->addView(gridLayout);

    for (int i = 0; i < 40; ++i) {
        auto imageView = new AndroidImageView();
        imageView->setContext(this->context);
        if (i % 2== 0) {
            imageView->setSource("bird.gif");
        } else {
            imageView->setSource("raining.png");
        }
        imageView->setWidth(400);
        imageView->setHeight(250);
        gridLayout->addView(imageView);
    }
}

}
