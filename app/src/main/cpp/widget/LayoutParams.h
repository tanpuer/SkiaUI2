#pragma once

#include <yoga/YGEnums.h>
#include "array"
#include "MeasureSpec.h"

/**
 * 布局相关参数尽量都放在LayoutParams中
 */
class LayoutParams {

public:

    LayoutParams();

    LayoutParams(LayoutParams &other) = delete;

    LayoutParams(LayoutParams &&other) = delete;

    LayoutParams &operator=(LayoutParams &other) = delete;

    LayoutParams &operator=(LayoutParams &&other) = delete;

    ~LayoutParams();

    static LayoutParams *makeEmpty();

    static LayoutParams *makeExactlyLayoutParams(int w, int h);

    static LayoutParams *makeExactlyWidth(int w);

    static LayoutParams *makeExactlyHeight(int h);

    static LayoutParams *makeWrapContent(bool w = true, bool h = true);

    static LayoutParams *makeMatchParent(bool w = true, bool h = true);

public:

    int _width, _height;

    int _widthMode, _heightMode;

    int _marginLeft, _marginRight, _marginTop, _marginBottom;

    int _paddingStart, _paddingEnd, _paddingTop, _paddingBottom;

    void setMargin(int margin);

    void setMargin(const std::array<int, 4> &margins);

    void setPadding(int padding);

    void setPadding(const std::array<int, 4> &paddings);

};
