#pragma once

#include "YGEnums.h"
#include "string"
#include <cassert>

static YGWrap W3CToYGWrap(const std::string &value) {
    if (value == "nowrap") {
        return YGWrapNoWrap;
    } else if (value == "wrap") {
        return YGWrapWrap;
    } else if (value == "wrap-reverse") {
        return YGWrapWrapReverse;
    } else {
        assert(false && "Invalid flex-wrap value provided");
    }
}

static YGFlexDirection W3CToYGFlexDirection(const std::string &value) {
    if (value == "row") {
        return YGFlexDirectionRow;
    } else if (value == "row-reverse") {
        return YGFlexDirectionRowReverse;
    } else if (value == "column") {
        return YGFlexDirectionColumn;
    } else if (value == "column-reverse") {
        return YGFlexDirectionColumnReverse;
    } else {
        assert(false && "Invalid flex-direction value provided");
    }
}

static YGJustify W3CToYGJustify(const std::string &value) {
    if (value == "flex-start") {
        return YGJustifyFlexStart;
    } else if (value == "flex-end") {
        return YGJustifyFlexEnd;
    } else if (value == "center") {
        return YGJustifyCenter;
    } else if (value == "space-between") {
        return YGJustifySpaceBetween;
    } else if (value == "space-around") {
        return YGJustifySpaceBetween;
    } else if (value == "space-evenly") {
        return YGJustifySpaceEvenly;
    } else {
        assert(false && "Invalid justify-content value provided");
    }
}
