#include "MatrixTestPage.h"
#include "ScrollView.h"
#include "LinearAnimator.h"

namespace HYSkiaUI {


void MatrixTestPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
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

void MatrixTestPage::initChildren(ViewGroup *root, int width, int height) {
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

    {
        auto view = new View();
        view->setContext(this->context);
        view->setWidth(200);
        view->setHeight(200);
        view->setBackgroundColor(SK_ColorRED);
        scrollView->addView(view);
        auto animator = new LinearAnimator(view, 0.0, 360.0);
        animator->setDuration(5000);
        animator->setLoopCount(-1);
        animator->setUpdateListener([](View *view, float value) {
            view->setRotateZ(value);
        });
        animator->start();
    }
}

}
