#pragma once

#include <cstdint>
#include <cassert>
#include <yoga/Yoga.h>
#include "array"
#include "memory"
#include "vector"
#include "core/SkPaint.h"
#include "core/SkCanvas.h"
#include "core/SkRRect.h"
#include "TouchEventDispatcher.h"
#include "IAnimator.h"
#include "core/SkBlurTypes.h"
#include "SkiaUIContext.h"
#include "MeasureTime.h"
#include "v8.h"

static int64_t VIEW_ID = 0;

namespace HYSkiaUI {

struct ResolutionUniforms {
    float width;
    float height;
};

class View {

public:

    View();

    virtual ~View();

#pragma mark View

    virtual const char *name();

    virtual void measure();

    virtual void setMeasuredDimension(int _measuredWidth, int _measuredHeight);

    virtual void layout(int l, int t, int r, int b);

    virtual void draw(SkCanvas *canvas);

    virtual bool isViewGroup();

    virtual bool isScroller();

    const std::shared_ptr<SkiaUIContext> &getContext();

    virtual void setContext(std::shared_ptr<SkiaUIContext> &context);

    virtual View *getParent();

    virtual void setParent(View *parent);

    virtual void removeFromParent();

    virtual int64_t getViewId();

    virtual void setOnClickListener(std::function<void(View *)> clickListener);

    virtual const std::function<void(View *)> &getClickListener();

    virtual void performClick();

    virtual void setVelocity(float x, float y);

    virtual void onShow();

    virtual void onHide();

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent);

    virtual bool onTouchEvent(TouchEvent *touchEvent);

    virtual void requestDisallowInterceptTouchEvent(bool disallowIntercept);

    virtual void setCustomTouchEventDispatcher(TouchEventDispatcher *touchEventDispatcher);

    virtual void performAnimations();

    void setAnimator(IAnimator *animator);

    virtual void onSizeChange(int width, int height);

    virtual const Page* getPage();

    //TODO
    float animTranslateX = 0.0f;
    float animTranslateY = 0.0f;

protected:

    View *parent = nullptr;

    Page *page = nullptr;

    int64_t viewId;

    bool needToMeasure = false;

    bool isDirty = false;

    std::function<void(View *)> viewClickListener = nullptr;

    std::shared_ptr<SkiaUIContext> context = nullptr;

    float xVelocity = 0.0f;
    float yVelocity = 0.0f;

    std::unique_ptr<TouchEventDispatcher> touchEventDispatcher;

    std::unordered_map<uint32_t, std::unique_ptr<IAnimator>> animators;

public:

#pragma mark yoga

    virtual void setAlignSelf(YGAlign align);

    virtual void setPositionType(YGPositionType type);

    YGPositionType getPositionType();

    virtual void setDisplay(YGDisplay display);

    virtual void setFlexGrow(float grow);

    virtual void setGap(const YGGutter gutter, const float gapLength);

    virtual void setFlex(float flex);

    virtual float getFlex();

    virtual void setWidth(int width);

    virtual void setHeight(int height);

    virtual void markDirty();

    virtual void clearDirty();

    virtual void markMeasure();

    virtual void clearMeasure();

    virtual int getHeight();

    virtual int getWidth();

    int getLeft();

    int getTop();

    int getRight();

    int getBottom();

    virtual YGNodeRef getNode();

    virtual void setMargin(std::vector<int> margins);

    virtual void setPadding(std::vector<int> paddings);

    virtual void setMarginTop(int marginTop);

    virtual int getMarginTop();

    virtual void setMarginLeft(int marginLeft);

    virtual int getMarginLeft();

    virtual void setMarginRight(int marginRight);

    virtual int getMarginRight();

    virtual void setMarginBottom(int marginBottom);

    virtual int getMarginBottom();

    virtual void setAspectRatio(float ratio);

    virtual void setWidthPercent(float widthPercent);

    virtual void setHeightPercent(float heightPercent);

    virtual void setTranslateX(float translateX);

    virtual void setTranslateY(float translateY);

    virtual void setMinSize(int minWidth, int minHeight);

protected:

    YGNodeRef node = nullptr;

    YGConfigRef config = nullptr;

    float flex = 0.0f;

    int width = 0;
    int height = 0;

    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;

    int minWidth = 0;
    int minHeight = 0;

    float translateX = 0.0f;
    float translateY = 0.0f;

    float widthPercent = 0.0f;
    float heightPercent = 0.0f;

    int marginLeft = 0;
    int marginTop = 0;
    int marginRight = 0;
    int marginBottom = 0;

    int paddingLeft = 0;
    int paddingTop = 0;
    int paddingRight = 0;
    int paddingBottom = 0;

public:

#pragma mark skia

    virtual void setBackgroundColor(SkColor color);

    virtual void setBackgroundColor(const std::string &hexColor);

    virtual const char *getBackgroundColor();

    virtual void setAntiAlias(bool antiAlias);

    virtual void setStyle(SkPaint::Style style);

    virtual void setStrokeWidth(SkScalar _width);

    virtual void setCornerRadius(int radius);

    virtual void setAlpha(float alpha);

    virtual float getAlpha();

    virtual void setLinearGradient(std::vector<SkColor> colors);

    virtual void setSwiperGradient(std::vector<SkColor> colors);

    virtual void setBlurMask(SkBlurStyle style, SkScalar sigma);

    virtual void setBlur(float blur);

    const SkRect &getRect();

    float getRotateZ();

    void setRotateZ(float z);

    float getScaleX();

    void setScaleX(float scale);

    float getScaleY();

    void setScaleY(float scale);

    void setTransX(float transX);

    float getTransX();

    void setTransY(float transY);

    float getTransY();

    virtual bool isTouchInRect(float x, float y);

    virtual void multiplyParentViewMatrix();

protected:

    std::unique_ptr<SkPaint> paint;

    SkRect skRect;

    SkRRect rRect;

    int cornerRadius = 0;

    SkRect skRectWithBorder;

    std::vector<SkColor> linearGradientColors;

    std::vector<SkColor> swiperGradientColors;

    std::string backgroundColor;

    SkMatrix viewMatrix;

    float rotateX = 0.0f;
    float rotateY = 0.0f;
    float rotateZ = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float transX = 0.0f;
    float transY = 0.0f;

    SkPoint leftTop = {0.0f, 0.0f};
    SkPoint rightTop = {0.0f, 0.0f};
    SkPoint leftBottom = {0.0f, 0.0f};
    SkPoint rightBottom = {0.0f, 0.0f};

#pragma mark v8
public:

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> clickFunction;

#pragma mark Compose
public:

    virtual void checkJavaViewRef(jobject instance);

    const jobject getJavaViewRef();

protected:

    jobject globalJavaViewRef = nullptr;

};

}
