#include "FlexboxLayout.h"

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
    if (_direction == YGFlexDirectionRow) {
        layoutHorizontal(l, t, r, b);
    } else {
        layoutVertical(l, t, r, b);
    }
}

void FlexboxLayout::layoutVertical(int l, int t, int r, int b) {
    for (auto &child: children) {
        auto left = static_cast<int>(YGNodeLayoutGetLeft(child->node));
        auto top = static_cast<int>(YGNodeLayoutGetTop(child->node));
        auto width = static_cast<int>(YGNodeLayoutGetWidth(child->node));
        auto height = static_cast<int>(YGNodeLayoutGetHeight(child->node));
//        ALOGD("FlexboxLayout: layout vertical %s %d %d %d %d", child->name(), left, top, width, height)
        child->layout(left + l, top + t, left + l + width, top + t + height);
    }
}

void FlexboxLayout::layoutHorizontal(int l, int t, int r, int b) {
    for (auto &child: children) {
        auto left = static_cast<int>(YGNodeLayoutGetLeft(child->node));
        auto top = static_cast<int>(YGNodeLayoutGetTop(child->node));
        auto width = static_cast<int>(YGNodeLayoutGetWidth(child->node));
        auto height = static_cast<int>(YGNodeLayoutGetHeight(child->node));
//        ALOGD("FlexboxLayout: layout horizontal %s %d %d %d %d", child->name(), left, top, width, height)
        child->layout(left + l, top + t, left + l + width, top + t + height);
    }
}
