//
// Created by cw on 2/11/22.
//

#include "LayoutParams.h"

LayoutParams::LayoutParams() : _width(0), _height(0),
                               _heightMode(YGMeasureModeUndefined),
                               _widthMode(YGMeasureModeUndefined),
                               _marginLeft(0), _marginRight(0), _marginTop(0), _marginBottom(0),
                               _paddingStart(0), _paddingEnd(0), _paddingTop(0), _paddingBottom(0) {

}

LayoutParams::~LayoutParams() = default;

LayoutParams *LayoutParams::makeEmpty() {
    return new LayoutParams();
}

LayoutParams *LayoutParams::makeExactlyLayoutParams(int w, int h) {
    auto layoutParams = new LayoutParams();
    layoutParams->_width = w;
    layoutParams->_height = h;
    layoutParams->_widthMode = EXACTLY;
    layoutParams->_heightMode = EXACTLY;
    return layoutParams;
}

LayoutParams *LayoutParams::makeExactlyWidth(int w) {
    auto layoutParams = new LayoutParams();
    layoutParams->_width = w;
    layoutParams->_height = 0;
    layoutParams->_widthMode = EXACTLY;
    layoutParams->_heightMode = UNSPECIFIED;
    return layoutParams;
}

LayoutParams *LayoutParams::makeExactlyHeight(int h) {
    auto layoutParams = new LayoutParams();
    layoutParams->_width = 0;
    layoutParams->_height = h;
    layoutParams->_widthMode = UNSPECIFIED;
    layoutParams->_heightMode = EXACTLY;
    return layoutParams;
}

LayoutParams *LayoutParams::makeWrapContent(bool w, bool h) {
    auto layoutParams = new LayoutParams();
    return layoutParams;
}

LayoutParams *LayoutParams::makeMatchParent(bool w, bool h) {
    auto layoutParams = new LayoutParams();
    return layoutParams;
}

void LayoutParams::setMargin(int margin) {
    _marginLeft = margin;
    _marginRight = margin;
    _marginTop = margin;
    _marginBottom = margin;
}

void LayoutParams::setPadding(int padding) {
    _paddingStart = padding;
    _paddingEnd = padding;
    _paddingTop = padding;
    _paddingBottom = padding;
}

void LayoutParams::setMargin(const std::array<int, 4> &margins) {
    _marginLeft = margins[0];
    _marginTop = margins[1];
    _marginRight = margins[2];
    _marginBottom = margins[3];
}

void LayoutParams::setPadding(const std::array<int, 4> &paddings) {
    _paddingStart = paddings[0];
    _paddingTop = paddings[1];
    _paddingEnd = paddings[2];
    _paddingBottom = paddings[3];
}
