#include <base/native_log.h>
#include <yoga/Utils.h>
#include "View.h"
#include "effects/SkCornerPathEffect.h"
#include "core/SkPathEffect.h"
#include "SkiaUIContext.h"
#include "effects/SkGradientShader.h"
#include "core/SkMaskFilter.h"
#include "core/SkBlurTypes.h"
#include "color_util.h"

namespace HYSkiaUI {

View::View() : width(0.0), height(0.0), skRect(SkIRect::MakeEmpty()), cornerRadius(0),
               skRectWithBorder(SkRect::MakeEmpty()),
               minWidth(0), minHeight(0),
               parentId(0),
               marginLeft(0), marginTop(0), marginRight(0), marginBottom(0),
               paddingLeft(0), paddingTop(0), paddingRight(0), paddingBottom(0),
               isDirty(false),
               widthPercent(0.0f), heightPercent(0.0f), viewMatrix(SkMatrix::I()) {
    viewId = VIEW_ID++;
    paint = std::make_unique<SkPaint>();
    paint->setAntiAlias(true);
    paint->setColor(SK_ColorWHITE);
    touchEventDispatcher = std::make_unique<TouchEventDispatcher>(this);
}

View::~View() {
    if (node != nullptr) {
        YGNodeFree(node);
    }
    viewLayoutCallback = nullptr;
    viewClickListener = nullptr;
    clickFunction.Reset();
    if (globalJavaViewRef) {
        getContext()->getJniEnv()->DeleteGlobalRef(globalJavaViewRef);
    }
}

#pragma mark yoga

void View::measure() {

}

void View::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    width = _measuredWidth;
    height = _measuredHeight;
    YGNodeStyleSetWidth(node, _measuredWidth);
    YGNodeStyleSetHeight(node, _measuredHeight);
}

void View::layout(int l, int t, int r, int b) {
    skRect.setLTRB(l, t, r, b);
    left = l;
    top = t;
    right = r;
    bottom = b;
    width = r - l;
    height = b - t;

    if (viewLayoutCallback != nullptr) {
        viewLayoutCallback(l, t, r, b);
    }
    if (!linearGradientColors.empty()) {
        SkPoint points[2]{SkPoint::Make(l, t), SkPoint::Make(r, b)};
        auto gradientShader = SkGradientShader::MakeLinear(
                points,
                linearGradientColors.data(),
                nullptr,
                linearGradientColors.size(),
                SkTileMode::kClamp
        );
        paint->setShader(std::move(gradientShader));
    }
    if (!swiperGradientColors.empty()) {
        auto gradientShader = SkGradientShader::MakeSweep(
                (l + r) / 2, (t + b) / 2,
                swiperGradientColors.data(),
                nullptr,
                swiperGradientColors.size());
        paint->setShader(std::move(gradientShader));
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
    markDirty();
}

bool View::isViewGroup() {
    return false;
}

bool View::isScroller() {
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
    markDirty();
    backgroundColor = SkColorToString(color);
}

void View::setAntiAlias(bool antiAlias) {
    SkASSERT(paint);
    paint->setAntiAlias(antiAlias);
    markDirty();
}

void View::setStyle(SkPaint::Style style) {
    SkASSERT(paint);
    paint->setStyle(style);
    markDirty();
}

void View::setCornerRadius(int radius) {
    SkASSERT(paint);
    if (radius <= 0) {
        ALOGE("radius must > 0")
        return;
    }
    cornerRadius = radius;
    paint->setPathEffect(SkCornerPathEffect::Make(static_cast<SkScalar>(radius)));
    markDirty();
}

void View::setStrokeWidth(SkScalar _width) {
    SkASSERT(paint);
    if (_width < 0.0f) {
        ALOGE("width must > 0.0")
        return;
    }
    paint->setStrokeWidth(_width);
    markDirty();
}

void View::setAlpha(float alpha) {
    SkASSERT(paint);
    //if the backgroundColor is transparent, can not set alpha
    if (SkColorGetA(paint->getColor()) == 0) {
        return;
    }
    paint->setAlphaf(alpha);
    markDirty();
}

float View::getAlpha() {
    SkASSERT(paint);
    return paint->getAlphaf();
}

void View::setLinearGradient(std::vector<SkColor> colors) {
    linearGradientColors = std::move(colors);
    markDirty();
}

void View::setSwiperGradient(std::vector<SkColor> colors) {
    swiperGradientColors = std::move(colors);
    markDirty();
}

void View::setBlurMask(SkBlurStyle style, SkScalar sigma) {
    auto filter = SkMaskFilter::MakeBlur(style, sigma);
    paint->setMaskFilter(filter);
    markDirty();
}

void View::setWidthPercent(float widthPercent) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    this->widthPercent = widthPercent;
    YGNodeStyleSetWidthPercent(node, widthPercent);
    markDirty();
}

void View::setHeightPercent(float heightPercent) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    this->heightPercent = heightPercent;
    YGNodeStyleSetHeightPercent(node, heightPercent);
    markDirty();
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

void View::setLayoutCallback(std::function<void(int, int, int, int)> callback) {
    viewLayoutCallback = callback;
    markDirty();
}

void View::removeLayoutCallback() {
    viewLayoutCallback = nullptr;
    markDirty();
}

void View::setOnClickListener(std::function<void(View *)> clickListener) {
    viewClickListener = clickListener;
}

void View::removeClickListener() {
    viewClickListener = nullptr;
    markDirty();
}

void View::performClick() {
    if (viewClickListener != nullptr) {
        viewClickListener(this);
    }
}

std::function<void(View *)> View::getClickListener() {
    return viewClickListener;
}

void View::setPositionType(YGPositionType type) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetPositionType(node, type);
    markDirty();
}

void View::setDisplay(YGDisplay display) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetDisplay(node, display);
    markDirty();
}

void View::setFlexGrow(float grow) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetFlexGrow(node, grow);
    markDirty();
}

void View::setGap(const YGGutter gutter, const float gapLength) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetGap(node, gutter, gapLength);
    markDirty();
}

void View::setFlex(float flex) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetFlex(node, flex);
    markDirty();
}

float View::getFlex() {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return 0;
    }
    return YGNodeStyleGetFlex(node);
}

void View::setWidth(int width) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    this->width = width;
    YGNodeStyleSetWidth(node, width);
}

void View::setHeight(int height) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    this->height = height;
    YGNodeStyleSetHeight(node, height);
}

void View::setMargin(std::vector<int> margins) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    marginLeft = margins[0];
    marginTop = margins[1];
    marginRight = margins[2];
    marginBottom = margins[3];
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, marginLeft);
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, marginTop);
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, marginRight);
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, marginBottom);
    markDirty();
}

void View::setPadding(std::vector<int> paddings) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    paddingLeft = paddings[0];
    paddingTop = paddings[1];
    paddingRight = paddings[2];
    paddingBottom = paddings[3];
    YGNodeStyleSetPadding(node, YGEdge::YGEdgeLeft, paddings[0]);
    YGNodeStyleSetPadding(node, YGEdge::YGEdgeTop, paddings[1]);
    YGNodeStyleSetPadding(node, YGEdge::YGEdgeRight, paddings[2]);
    YGNodeStyleSetPadding(node, YGEdge::YGEdgeBottom, paddings[3]);
    markDirty();
}

void View::setAspectRatio(float ratio) {
    YGAssert(node, "view is null, pls check");
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetAspectRatio(node, ratio);
    markDirty();
}

const std::shared_ptr<SkiaUIContext> View::getContext() {
    return context;
}

void View::setContext(std::shared_ptr<SkiaUIContext> context) {
    this->context = context;
    if (this->config != nullptr) {
        ALOGD("multi set config error, pls check")
        return;
    }
    this->config = context->getConfig();
    //todo setConfig之后才会进行node的创建
    node = YGNodeNewWithConfig(config);
}

void View::setBackgroundColor(const std::string &hexColor) {
    backgroundColor = hexColor;
    int r, g, b, a;
    hexToRGBA(hexColor, r, g, b, a);
    setBackgroundColor(SkColorSetARGB(a, r, g, b));
}

const char *View::getBackgroundColor() {
    return backgroundColor.c_str();
}

void View::onShow() {

}

void View::onHide() {

}

void View::markDirty() {
    isDirty = true;
    getContext()->markDirty();
}

void View::clearDirty() {
    isDirty = false;
}

void View::setAnimator(IAnimator *animator) {
    animators[animator->getAnimatorId()] = std::unique_ptr<IAnimator>(animator);
}

void View::setVelocity(float x, float y) {
    this->xVelocity = x;
    this->yVelocity = y;
}

void View::performAnimations() {
    std::vector<uint32_t> needReset;
    for (const auto &animator: animators) {
        if (animator.second->isEnd()) {
            needReset.push_back(animator.first);
        } else {
            animator.second->update(skRect);
        }
    }
    for (auto &index: needReset) {
        animators.erase(index);
    }
}

void View::checkJavaViewRef(jobject instance) {
    if (globalJavaViewRef == nullptr) {
        globalJavaViewRef = getContext()->getJniEnv()->NewGlobalRef(instance);
    }
}

jobject View::getJavaViewRef() {
    return globalJavaViewRef;
}

View *View::getParent() {
    return parent;
}

void View::markMeasure() {
    needToMeasure = true;
}

void View::clearMeasure() {
    needToMeasure = false;
}

void View::setMarginTop(int marginTop) {
    this->marginTop = marginTop;
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, marginTop);
    markDirty();
}

int View::getMarginTop() {
    return marginTop;
}

void View::setMarginLeft(int marginLeft) {
    this->marginLeft = marginLeft;
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, marginLeft);
    markDirty();
}

int View::getMarginLeft() {
    return marginLeft;
}

void View::setMarginRight(int marginRight) {
    this->marginRight = marginRight;
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, marginRight);
    markDirty();
}

int View::getMarginRight() {
    return marginRight;
}

void View::setMarginBottom(int marginBottom) {
    this->marginBottom = marginBottom;
    YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, marginBottom);
    markDirty();
}

int View::getMarginBottom() {
    return marginBottom;
}

YGPositionType View::getPositionType() {
    return YGNodeStyleGetPositionType(node);
}

}
