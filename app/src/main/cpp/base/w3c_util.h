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
