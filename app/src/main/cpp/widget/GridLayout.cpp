#include "GridLayout.h"

namespace HYSkiaUI {

GridLayout::GridLayout() {

}

GridLayout::~GridLayout() {

}

void GridLayout::setContext(std::shared_ptr<SkiaUIContext> &context) {
    View::setContext(context);
    setFlexWrap(YGWrap::YGWrapWrap);
    FlexboxLayout::setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
}

void GridLayout::measure() {
    if (width == 0 || height == 0) {
        return;
    }
    for (auto &child: children) {
        child->setWidth(width / count);
    }
    FlexboxLayout::measure();
}

void GridLayout::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    layoutChildren(l, t, r, b);
}

void GridLayout::setRowCount(int count) {
    this->count = count;
    markDirty();
}

}
