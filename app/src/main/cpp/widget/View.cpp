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
#include "ViewGroup.h"

namespace HYSkiaUI {

View::View() : skRect(SkRect::MakeEmpty()), skRectWithBorder(SkRect::MakeEmpty()),
               viewMatrix(SkMatrix::I()) {
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
    animators.clear();
    viewClickListener = nullptr;
    clickFunction.Reset();
    if (globalJavaViewRef) {
        getContext()->getJniEnv()->DeleteGlobalRef(globalJavaViewRef);
    }
}

const char *View::name() {
    return "View";
}

void View::measure() {

}

void View::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    width = _measuredWidth;
    height = _measuredHeight;
    YGNodeStyleSetWidth(node, _measuredWidth);
    YGNodeStyleSetHeight(node, _measuredHeight);
}

void View::layout(int l, int t, int r, int b) {
    if (skRect.width() != r - l || skRect.height() != b - t) {
        onSizeChange(r - l, b - t);
    }
    skRect.setLTRB(l, t, r, b);
    rRect.setRectXY(skRect, cornerRadius, cornerRadius);
    left = l;
    top = t;
    right = r;
    bottom = b;
    width = r - l;
    height = b - t;

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
    viewMatrix.setIdentity();
    viewMatrix.preTranslate(transX, transY);
    viewMatrix.preScale(scaleX, scaleY, left + width / 2, top + height / 2);
    viewMatrix.preRotate(rotateZ, left + width / 2, top + height / 2);
    leftTop.set(left, top);
    rightTop.set(right, top);
    leftBottom.set(left, bottom);
    rightBottom.set(right, bottom);
}

void View::draw(SkCanvas *canvas) {
    multiplyParentViewMatrix();
    canvas->save();
    canvas->setMatrix(viewMatrix);
    if (cornerRadius > 0) {
        canvas->clipRRect(rRect);
    }
    if (YGFloatsEqual(paint->getStrokeWidth(), 0.0f)) {
        canvas->drawRect(skRect, *paint);
    } else {
        //view border like Android, help to development
        auto diff = (paint->getStrokeWidth()) / 2;
        skRectWithBorder.setLTRB(skRect.left() + diff, skRect.top() + diff, skRect.right() - diff,
                                 skRect.bottom() - diff);
        if (width == height && width == cornerRadius * 2) {
            canvas->drawRoundRect(skRectWithBorder, cornerRadius, cornerRadius, *paint);
        } else {
            canvas->drawRect(skRectWithBorder, *paint);
        }
    }
    canvas->restore();
}

void View::setAlignSelf(YGAlign align) {
    SkASSERT(node);
    YGNodeStyleSetAlignSelf(node, align);
    markDirty();
}

bool View::isViewGroup() {
    return false;
}

bool View::isScroller() {
    return false;
}

int View::getHeight() {
    return height;
}

int View::getWidth() {
    return width;
}

int View::getLeft() {
    return left;
}

int View::getTop() {
    return top;
}

int View::getRight() {
    return right;
}

int View::getBottom() {
    return bottom;
}

YGNodeRef View::getNode() {
    return node;
}

void View::setBackgroundColor(SkColor color) {
    paint->setColor(color);
    markDirty();
    backgroundColor = SkColorToString(color);
}

void View::setAntiAlias(bool antiAlias) {
    paint->setAntiAlias(antiAlias);
    markDirty();
}

void View::setStyle(SkPaint::Style style) {
    paint->setStyle(style);
    markDirty();
}

void View::setCornerRadius(int radius) {
    if (radius <= 0) {
        ALOGE("radius must > 0")
        return;
    }
    cornerRadius = radius;
    markDirty();
}

void View::setStrokeWidth(SkScalar _width) {
    if (_width < 0.0f) {
        ALOGE("width must > 0.0")
        return;
    }
    paint->setStrokeWidth(_width);
    markDirty();
}

void View::setAlpha(float alpha) {
    //if the backgroundColor is transparent, can not set alpha
    if (SkColorGetA(paint->getColor()) == 0) {
        return;
    }
    paint->setAlphaf(alpha);
    markDirty();
}

float View::getAlpha() {
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

const SkRect &View::getRect() {
    return skRect;
}

void View::setWidthPercent(float widthPercent) {
    if (node == nullptr) {
        return;
    }
    this->widthPercent = widthPercent;
    YGNodeStyleSetWidthPercent(node, widthPercent);
    markDirty();
}

void View::setHeightPercent(float heightPercent) {
    if (node == nullptr) {
        return;
    }
    this->heightPercent = heightPercent;
    YGNodeStyleSetHeightPercent(node, heightPercent);
    markDirty();
}

void View::setTranslateX(float translateX) {
    this->translateX = translateX;
}

void View::setTranslateY(float translateY) {
    this->translateY = translateY;
}

void View::setMinSize(int minWidth, int minHeight) {
    this->minWidth = minWidth;
    this->minHeight = minHeight;
    YGNodeStyleSetMinWidth(node, minWidth);
    YGNodeStyleSetMinHeight(node, minHeight);
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

void View::setOnClickListener(std::function<void(View *)> clickListener) {
    viewClickListener = std::move(clickListener);
}

void View::performClick() {
    if (viewClickListener != nullptr) {
        viewClickListener(this);
    }
}

const std::function<void(View *)> &View::getClickListener() {
    return viewClickListener;
}

void View::setPositionType(YGPositionType type) {
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetPositionType(node, type);
    markDirty();
}

void View::setDisplay(YGDisplay display) {
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetDisplay(node, display);
    markDirty();
}

void View::setFlexGrow(float grow) {
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetFlexGrow(node, grow);
    markDirty();
}

void View::setGap(const YGGutter gutter, const float gapLength) {
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetGap(node, gutter, gapLength);
    markDirty();
}

void View::setFlex(float flex) {
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetFlex(node, flex);
    markDirty();
}

float View::getFlex() {
    if (node == nullptr) {
        return 0;
    }
    return YGNodeStyleGetFlex(node);
}

void View::setWidth(int width) {
    if (node == nullptr) {
        return;
    }
    this->width = width;
    YGNodeStyleSetWidth(node, width);
    markDirty();
}

void View::setHeight(int height) {
    if (node == nullptr) {
        return;
    }
    this->height = height;
    YGNodeStyleSetHeight(node, height);
    markDirty();
}

void View::setMargin(std::vector<int> margins) {
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
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetAspectRatio(node, ratio);
    markDirty();
}

const std::shared_ptr<SkiaUIContext> &View::getContext() {
    return context;
}

void View::setContext(std::shared_ptr<SkiaUIContext> &context) {
    this->context = context;
    if (this->config != nullptr) {
        ALOGD("multi set config error, pls check")
        return;
    }
    this->config = context->getConfig();
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

void View::onSizeChange(int width, int height) {

}

void View::setVelocity(float x, float y) {
    this->xVelocity = x;
    this->yVelocity = y;
}

void View::performAnimations() {
    for (auto itr = animators.begin(); itr != animators.end();) {
        if (itr->second == nullptr || itr->second->isEnd()) {
            itr = animators.erase(itr);
        } else {
            itr->second->update(skRect);
            ++itr;
        }
    }
}

void View::checkJavaViewRef(jobject instance) {
    if (globalJavaViewRef == nullptr) {
        globalJavaViewRef = getContext()->getJniEnv()->NewGlobalRef(instance);
    }
}

const jobject View::getJavaViewRef() {
    return globalJavaViewRef;
}

View *View::getParent() {
    return parent;
}

void View::removeFromParent() {
    reinterpret_cast<ViewGroup *>(parent)->removeView(this);
}

void View::setParent(HYSkiaUI::View *parent) {
    this->parent = parent;
}

int64_t View::getViewId() {
    return viewId;
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

float View::getRotateZ() {
    return rotateZ;
}

void View::setRotateZ(float z) {
    rotateZ = z;
    markDirty();
}

float View::getScaleX() {
    return scaleX;
}

void View::setScaleX(float scale) {
    scaleX = scale;
    markDirty();
}

float View::getScaleY() {
    return scaleY;
}

void View::setScaleY(float scale) {
    scaleY = scale;
    markDirty();
}

float View::getTransX() {
    return transX;
}

void View::setTransX(float transX) {
    this->transX = transX;
    markDirty();
}

void View::setTransY(float transY) {
    this->transY = transY;
    markDirty();
}

float View::getTransY() {
    return transY;
}

bool View::isTouchInRect(float x, float y) {
    if (viewMatrix.isIdentity()) {
        return x >= left && x <= left + width && y >= top && y <= top + height;
    } else {
        SkPoint pt = {x, y};
        const SkPoint quad[4] = {leftTop, rightTop, rightBottom, leftBottom};
        int prev_side = 0;
        for (int i = 0; i < 4; i++) {
            SkPoint current = quad[i];
            SkPoint next = quad[(i + 1) % 4];
            SkVector edge = next - current;
            SkVector pointVec = pt - current;
            float cross = edge.x() * pointVec.y() - edge.y() * pointVec.x();
            if (cross == 0) return true;
            int current_side = (cross > 0) ? 1 : -1;
            if (prev_side == 0) {
                prev_side = current_side;
            } else if (prev_side != current_side) {
                return false;
            }
        }
        return true;
    }
}

void View::multiplyParentViewMatrix() {
    if (getParent() != nullptr) {
        viewMatrix = getParent()->viewMatrix * viewMatrix;
    }
    if (!viewMatrix.isIdentity()) {
        leftTop = viewMatrix.mapPoint(leftTop);
        rightTop = viewMatrix.mapPoint(rightTop);
        leftBottom = viewMatrix.mapPoint(leftBottom);
        rightBottom = viewMatrix.mapPoint(rightBottom);
    }
}

}
