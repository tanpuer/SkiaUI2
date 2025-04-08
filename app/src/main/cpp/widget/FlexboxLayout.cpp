#include "FlexboxLayout.h"

namespace HYSkiaUI {

FlexboxLayout::FlexboxLayout() : ViewGroup() {

}

FlexboxLayout::~FlexboxLayout() {

}

const char *FlexboxLayout::name() {
    return "FlexboxLayout";
}

void FlexboxLayout::setFlexDirection(YGFlexDirection direction) {
    this->_direction = direction;
    ViewGroup::setFlexDirection(direction);
}

void FlexboxLayout::measure() {
    for (auto &child: children) {
        measureChild(child);
    }
}

void FlexboxLayout::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    layoutChildren(l, t, r, b);
}

void FlexboxLayout::layoutChildren(int l, int t, int r, int b) {
    for (auto &child: children) {
        auto childNode = child->getNode();
        auto left = static_cast<int>(YGNodeLayoutGetLeft(childNode));
        auto top = static_cast<int>(YGNodeLayoutGetTop(childNode));
        auto width = static_cast<int>(YGNodeLayoutGetWidth(childNode));
        auto height = static_cast<int>(YGNodeLayoutGetHeight(childNode));
        child->layout(left + l, top + t, left + l + width, top + t + height);
    }
}

}
