#pragma once

#include <cstdint>
#include <yoga/Yoga.h>
#include <memory>
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

    virtual const char *name() {
        return "view";
    }

    int64_t viewId;

    const char *parentName;

    int64_t parentId;

#pragma mark yoga 设置相关

    virtual void measure();

    virtual void setMeasuredDimension(int _measuredWidth, int _measuredHeight);

    virtual void layout(int l, int t, int r, int b);

    virtual void draw(SkCanvas *canvas);

    virtual bool isViewGroup();

    virtual bool isScroller();

    const std::shared_ptr<SkiaUIContext> getContext();

    virtual void setContext(std::shared_ptr<SkiaUIContext> context);

    /**
     * 在AlignItems的基础上自定义每个子视图的对齐方式
     * @param align
     */
    virtual void setAlignSelf(YGAlign align);

    virtual void setPositionType(YGPositionType type);

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

    virtual View *getParent();

    YGNodeRef node;

    YGConfigRef config = nullptr;

    int left, top, right, bottom = 0;

    bool markForDelete = false;

    View *parent = nullptr;

protected:

    int width, height;

    int minWidth, minHeight;

#pragma mark yoga 获取相关

public:

    virtual int getHeight();

    virtual int getWidth();

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

    std::unique_ptr<SkPaint> paint;

    int marginLeft, marginTop, marginRight, marginBottom;

    int paddingLeft, paddingTop, paddingRight, paddingBottom;

    SkIRect skRect;

    int cornerRadius;

    SkRect skRectWithBorder;

    virtual void setLinearGradient(std::vector<SkColor> colors);

    std::vector<SkColor> linearGradientColors;

    virtual void setSwiperGradient(std::vector<SkColor> colors);

    std::vector<SkColor> swiperGradientColors;

    virtual void setBlurMask(SkBlurStyle style, SkScalar sigma);

    virtual void setAspectRatio(float ratio);

public:

    virtual void setWidthPercent(float widthPercent);

    virtual void setHeightPercent(float heightPercent);

    float widthPercent, heightPercent;

#pragma mark TouchEvent

public:

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent);

    virtual bool onTouchEvent(TouchEvent *touchEvent);

    virtual void requestDisallowInterceptTouchEvent(bool disallowIntercept);

    virtual void setCustomTouchEventDispatcher(TouchEventDispatcher *touchEventDispatcher);

protected:

    std::unique_ptr<TouchEventDispatcher> touchEventDispatcher;

#pragma mark moving

public:

    float translateX = 0.0f;
    float translateY = 0.0f;

#pragma mark Animator

public:

    virtual void performAnimations();

protected:

    std::unordered_map<uint32_t, std::unique_ptr<IAnimator>> animators;

public:
    float animTranslateX = 0.0f;
    float animTranslateY = 0.0f;
    float rotateX = 0.0f;
    float rotateY = 0.0f;
    float rotateZ = 0.0f;

    void setAnimator(IAnimator *animator);

protected:

    bool isDirty;

    std::string backgroundColor;

    bool needToMeasure = false;

#pragma mark cakllbacks

public:

    virtual void setLayoutCallback(std::function<void(int, int, int, int)> callback);

    virtual void removeLayoutCallback();

    virtual void setOnClickListener(std::function<void(View *)> clickListener);

    virtual std::function<void(View *)> getClickListener();

    virtual void removeClickListener();

    virtual void performClick();

    virtual void setVelocity(float x, float y);

protected:

    std::function<void(int, int, int, int)> viewLayoutCallback = nullptr;

    std::function<void(View *)> viewClickListener = nullptr;

    std::shared_ptr<SkiaUIContext> context = nullptr;

    float xVelocity = 0.0f, yVelocity = 0.0f;

#pragma mark LifeCycle
public:
    virtual void onShow();

    virtual void onHide();

#pragma mark ClickEvent
public:
    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> clickFunction;

#pragma mark Compose
public:

    void checkJavaViewRef(jobject instance);

    jobject getJavaViewRef();

private:

    jobject globalJavaViewRef = nullptr;

};

}
