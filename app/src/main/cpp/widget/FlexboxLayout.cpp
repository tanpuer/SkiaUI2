#include "FlexboxLayout.h"

FlexboxLayout::FlexboxLayout() : ViewGroup() {

}

FlexboxLayout::~FlexboxLayout() {
//    ALOGD("delete FlexboxLayout %lld", viewId)
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
//    if (YGNodeStyleGetFlexDirection(node) == YGFlexDirectionRow) {
//        if (YGNodeStyleGetWidth(node).value > 0) {
//            YGNodeStyleSetWidth(node, YGNodeStyleGetWidth(node).value);
//        } else {
//            YGNodeStyleSetWidth(node, getChildWidthSum());
//        }
//        if (YGNodeStyleGetHeight(node).value > 0) {
//            YGNodeStyleSetHeight(node, YGNodeStyleGetHeight(node).value);
//        } else {
//            //no-wrap
//            YGNodeStyleSetHeight(node, getMaxHeightInChildren());
//        }
//    } else {
//        if (YGNodeStyleGetWidth(node).value > 0) {
//            YGNodeStyleSetWidth(node, YGNodeStyleGetWidth(node).value);
//        } else {
//            //no-wrap
//            YGNodeStyleSetWidth(node, getMaxWidthInChildren());
//        }
//        if (YGNodeStyleGetHeight(node).value > 0) {
//            YGNodeStyleSetHeight(node, YGNodeStyleGetHeight(node).value);
//        } else {
//            YGNodeStyleSetHeight(node, getChildHeightSum());
//        }
//    }
//    YGNodeCalculateLayout(node, YGNodeStyleGetWidth(node).value, YGNodeStyleGetHeight(node).value,
//                          YGDirectionLTR);
//    ViewGroup::setMeasuredDimension(YGNodeStyleGetWidth(node).value, YGNodeStyleGetHeight(node).value);
//    //如果是wrap，存在换行的情况，要重新计算FlexboxLayout的宽高
//    if (layoutParams->_heightMode != EXACTLY && _direction == YGFlexDirectionRow &&
//        YGNodeStyleGetFlexWrap(node) == YGWrapWrap) {
//        auto minTop = INT_MAX;
//        auto maxTop = INT_MIN;
//        for (auto &child: children) {
//            if (child->skRect.top() < minTop) {
//                minTop = child->skRect.top();
//            }
//            if (child->skRect.top() > maxTop) {
//                maxTop = child->skRect.top();
//            }
//        }
//        if (maxTop > minTop) {
//            YGNodeStyleSetHeight(node, YGNodeStyleGetHeight(node).value + maxTop - minTop);
//            ViewGroup::setMeasuredDimension(YGNodeStyleGetWidth(node).value, YGNodeStyleGetHeight(node).value + maxTop - minTop);
//        }
//    } else if (layoutParams->_widthMode != EXACTLY && _direction == YGFlexDirectionColumn &&
//               YGNodeStyleGetFlexWrap(node) == YGWrapWrap) {
//        auto minLeft = INT_MAX;
//        auto maxLeft = INT_MIN;
//        for (auto &child: children) {
//            if (child->skRect.left() < minLeft) {
//                minLeft = skRect.left();
//            }
//            if (child->skRect.left() > maxLeft) {
//                maxLeft = skRect.left();
//            }
//        }
//        if (maxLeft > minLeft) {
//            YGNodeStyleSetWidth(node, YGNodeStyleGetWidth(node).value + maxLeft - minLeft);
//            ViewGroup::setMeasuredDimension(YGNodeStyleGetWidth(node).value + maxLeft - minLeft, YGNodeStyleGetHeight(node).value);
//        }
//    }
}

void FlexboxLayout::layout(int l, int t, int r, int b) {
//    ALOGD("FlexboxLayout margin %d %d %d %d", marginLeft, marginTop, marginRight, marginBottom);
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
        //todo 需要考虑padding
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
        //todo 需要考虑padding
        child->layout(left + l, top + t, left + l + width, top + t + height);
    }
}

void FlexboxLayout::setFlexWrap(YGWrap wrap) {
    ViewGroup::setFlexWrap(wrap);
}

bool FlexboxLayout::addView(View *view) {
    return ViewGroup::addView(view);
}

bool FlexboxLayout::removeView(View *view) {
    return ViewGroup::removeView(view);
}
