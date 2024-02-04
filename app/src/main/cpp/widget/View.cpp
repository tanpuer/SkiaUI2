//
// Created by cw on 1/24/22.
//

#include <base/native_log.h>
#include <yoga/Utils.h>
#include "View.h"
#include "effects/SkCornerPathEffect.h"
#include "core/SkPathEffect.h"

View::View() : width(0.0), height(0.0), skRect(SkIRect::MakeEmpty()), cornerRadius(0),
               skRectWithBorder(SkRect::MakeEmpty()),
               minWidth(0), minHeight(0),
               parentId(0),
               marginLeft(0), marginTop(0), marginRight(0), marginBottom(0),
               isDirty(false),
               widthPercent(0.0f), hwRatio(0.0f) {
    viewId = VIEW_ID++;
    paint = new SkPaint();
    paint->setAntiAlias(true);
    touchEventDispatcher = std::make_unique<TouchEventDispatcher>(this);
    animator = std::unique_ptr<IAnimator>(nullptr);
}

View::~View() {
    delete paint;
    if (node != nullptr) {
        YGNodeFree(node);
    }
    viewLayoutCallback = nullptr;
    viewClickListener = nullptr;
}

#pragma mark yoga

/**
 * This is called to find out how big a view should be. The parent supplies constraint information in the width and height parameters.
The actual measurement work of a view is performed in onMeasure(int, int), called by this method. Therefore, only onMeasure(int, int) can and must be overridden by subclasses.
 * @param widthMeasureSpec
 * @param heightMeasureSpec
 */
void View::measure(int widthMeasureSpec, int heightMeasureSpec) {
    //todo 目前每次都是forceLayout Android子类只能override onMeasure方法，精简处理
    auto measuredWidth = getDefaultSize(minWidth, widthMeasureSpec);
    auto measuredHeight = getDefaultSize(minHeight, heightMeasureSpec);
    setMeasuredDimension(measuredWidth, measuredHeight);
//    ALOGD("setMeasuredDimension %s %d %d", name(), measuredWidth, measuredHeight)
}

void View::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    width = _measuredWidth;
    height = _measuredHeight;
    YGNodeStyleSetWidth(node, _measuredWidth);
    YGNodeStyleSetHeight(node, _measuredHeight);
}

int View::getDefaultSize(int minSize, int measureSpec) {
    int result = minSize;
    switch (MeasureSpec::getMode(measureSpec)) {
        case UNSPECIFIED: {
            result = minSize;
            break;
        }
        case AT_MOST:
        case EXACTLY: {
            result = MeasureSpec::getSize(measureSpec);
            break;
        }
        default: {
            break;
        }
    }
    return result;
}

void View::layout(int l, int t, int r, int b) {
    //todo 默认设置boarder位置 Android layout默认啥都不做
    skRect.setLTRB(l, t, r, b);
    width = r - l;
    height = b - t;
//    ALOGD("%s layout %d %d", name(), width, height)
    if (viewLayoutCallback != nullptr) {
        viewLayoutCallback(l, t, r, b);
    }
}

void View::draw(SkCanvas *canvas) {
    if (YGFloatsEqual(paint->getStrokeWidth(), 0.0f)) {
        canvas->drawIRect(skRect, *paint);
    } else {
        //view边框，辅助看大小
        auto diff = (paint->getStrokeWidth()) / 2;
        skRectWithBorder.setLTRB(skRect.left() + diff, skRect.top() + diff, skRect.right() - diff,
                                 skRect.bottom() - diff);
        if (width == height && width == cornerRadius * 2) {
            canvas->drawRoundRect(skRectWithBorder, cornerRadius, cornerRadius, *paint);
        } else {
            canvas->drawRect(skRectWithBorder, *paint);
        }
    }
}

void View::setAlignSelf(YGAlign align) {
    SkASSERT(node);
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetAlignSelf(node, align);
}

bool View::isViewGroup() {
    return false;
}

#pragma mark yoga 获取相关

int View::getHeight() {
    return height;
}

int View::getWidth() {
    return width;
}

#pragma mark skia

void View::setBackgroundColor(SkColor color) {
    SkASSERT(paint);
    paint->setColor(color);
}

void View::setAntiAlias(bool antiAlias) {
    SkASSERT(paint);
    paint->setAntiAlias(antiAlias);
}

void View::setStyle(SkPaint::Style style) {
    SkASSERT(paint);
    paint->setStyle(style);
}

void View::setCornerRadius(int radius) {
    SkASSERT(paint);
    if (radius <= 0) {
        ALOGE("radius must > 0")
        return;
    }
    cornerRadius = radius;
    paint->setPathEffect(SkCornerPathEffect::Make(static_cast<SkScalar>(radius)));
}

void View::setStrokeWidth(SkScalar _width) {
    SkASSERT(paint);
    if (_width < 0.0f) {
        ALOGE("width must > 0.0")
        return;
    }
    paint->setStrokeWidth(_width);
}

void View::setAlpha(float alpha) {
    SkASSERT(paint);
    paint->setAlphaf(alpha);
}

#pragma LayoutParams相关

void View::setLayoutParams(LayoutParams *_layoutParams) {
    this->marginLeft = _layoutParams->_marginLeft;
    this->marginTop = _layoutParams->_marginTop;
    this->marginRight = _layoutParams->_marginRight;
    this->marginBottom = _layoutParams->_marginBottom;
    layoutParams = std::unique_ptr<LayoutParams>(_layoutParams);
    YGNodeStyleSetMargin(node, YGEdgeLeft, layoutParams->_marginLeft);
    YGNodeStyleSetMargin(node, YGEdgeTop, layoutParams->_marginTop);
    YGNodeStyleSetMargin(node, YGEdgeRight, layoutParams->_marginRight);
    YGNodeStyleSetMargin(node, YGEdgeBottom, layoutParams->_marginBottom);
}

LayoutParams *View::getLayoutParams() {
    return layoutParams.get();
}

#pragma mark 后续才支持的

void View::setPadding(std::array<int, 4> paddings) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        ALOGE("YGNodeRef not initialized, pls check!")
        return;
    }
    YGNodeStyleSetPadding(node, YGEdgeLeft, paddings[0]);
    YGNodeStyleSetPadding(node, YGEdgeTop, paddings[1]);
    YGNodeStyleSetPadding(node, YGEdgeRight, paddings[2]);
    YGNodeStyleSetPadding(node, YGEdgeBottom, paddings[3]);
}

void View::setPadding(int padding) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetPadding(node, YGEdgeAll, padding);
}

void View::setSizePercent(float widthPercent, float hwRatio) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    this->widthPercent = widthPercent;
    this->hwRatio = hwRatio;
}

void View::setWidthAuto() {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetWidthAuto(node);
}

void View::setHeightAuto() {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetHeightAuto(node);
}

void View::setConfig(YGConfigRef config) {
    if (this->config != nullptr) {
        ALOGD("multi set config error, pls check")
        return;
    }
    this->config = config;
    //todo setConfig之后才会进行node的创建
    node = YGNodeNewWithConfig(config);
}

bool View::onInterceptTouchEvent(TouchEvent *touchEvent) {
    return touchEventDispatcher->onInterceptTouchEvent(touchEvent);
}

bool View::onTouchEvent(TouchEvent *touchEvent) {
    return touchEventDispatcher->onTouchEvent(touchEvent);
}

void View::requestDisallowInterceptTouchEvent(bool disallowIntercept) {
    touchEventDispatcher->requestDisallowInterceptTouchEvent(disallowIntercept);
}

void View::setCustomTouchEventDispatcher(TouchEventDispatcher *touchEventDispatcher) {
    ALOGD("setCustomTouchEventDispatcher")
    this->touchEventDispatcher = std::unique_ptr<TouchEventDispatcher>(touchEventDispatcher);
    this->touchEventDispatcher->setWeakView(this);
}

void View::handleAnimation() {
    if (animator != nullptr) {

    }
}

bool View::hasPercent() {
    return !YGFloatsEqual(0.0f, widthPercent) && YGFloatsEqual(0.0f, hwRatio);
}

void View::setLayoutCallback(std::function<void(int, int, int, int)> callback) {
    viewLayoutCallback = callback;
}

void View::removeLayoutCallback() {
    viewLayoutCallback = nullptr;
}

void View::setOnClickListener(std::function<void(View *)> clickListener) {
    viewClickListener = clickListener;
}

void View::removeClickListener() {
    viewClickListener = nullptr;
}

void View::performClick() {
    if (viewClickListener != nullptr) {
        viewClickListener(this);
    }
}

std::function<void(View *)> View::getClickListener() {
    return viewClickListener;
}

bool View::forceRequestTouchMove() {
    return false;
}
