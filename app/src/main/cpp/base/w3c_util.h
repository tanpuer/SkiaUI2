#pragma once

#include "YGEnums.h"
#include "string"
#include "ImageView.h"
#include <cassert>

using namespace HYSkiaUI;

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

static YGAlign W3CToYGAlign(const std::string &value) {
    if (value == "flex-start") {
        return YGAlignFlexStart;
    } else if (value == "center") {
        return YGAlignCenter;
    } else if (value == "flex-end") {
        return YGAlignFlexEnd;
    } else if (value == "stretch") {
        return YGAlignStretch;
    } else if (value == "baseline") {
        return YGAlignBaseline;
    } else if (value == "space-between") {
        return YGAlignSpaceBetween;
    } else if (value == "space-around") {
        return YGAlignSpaceAround;
    } else if (value == "auto") {
        return YGAlignAuto;
    } else {
        assert(false && "Invalid align-items value provided");
    }
}

static ImageView::ScaleType W3CToScaleType(const std::string &value) {
    if (value == "fill") {
        return ImageView::ScaleType::FitXY;
    } else if (value == "cover") {
        return ImageView::ScaleType::CenterCrop;
    } else if (value == "contain") {
        return ImageView::ScaleType::FitCenter;
    } else {
        assert(false && "Invalid object-fit value provided");
    }
}

static const char *scaleTypeToW3c(const ImageView::ScaleType scaleType) {
    if (scaleType == ImageView::ScaleType::FitXY) {
        return "fill";
    } else if (scaleType == ImageView::ScaleType::CenterCrop) {
        return "cover";
    } else if (scaleType == ImageView::ScaleType::FitCenter) {
        return "contain";
    } else {
        return "";
    }
}
