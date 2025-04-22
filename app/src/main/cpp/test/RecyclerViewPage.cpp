#include "RecyclerViewPage.h"

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
    auto recyclerView = new MyRecyclerView();
    recyclerView->setContext(context);
    recyclerView->setDataSize(100000);
    recyclerView->setWidth(width);
    recyclerView->setHeight(height);
    root->addView(recyclerView);
}

}
