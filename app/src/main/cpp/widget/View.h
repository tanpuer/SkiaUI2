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

static int64_t VIEW_ID = 0;

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

    virtual void setWidth(int width);

    virtual void setHeight(int height);

    YGNodeRef node;

    YGConfigRef config = nullptr;

    int left, top = 0;

protected:

    int width, height;

    int minWidth, minHeight;

#pragma mark yoga 获取相关

public:

    virtual int getHeight();

    virtual int getWidth();

#pragma mark skia

    virtual void setBackgroundColor(SkColor color);

    virtual void setAntiAlias(bool antiAlias);

    virtual void setStyle(SkPaint::Style style);

    virtual void setStrokeWidth(SkScalar _width);

    virtual void setCornerRadius(int radius);

    virtual void setAlpha(float alpha);

    virtual void setMargin(std::vector<int> margins);

    virtual void setPadding(std::vector<int> paddings);

    SkPaint *paint;

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
    //todo 后续才支持的
    /**
     * 绝大部分情况下，宽度根据屏幕宽度百分比设置，高度参考宽度，保持宽高比
     * @param widthPercent
     * @param hwRatio
     */
    virtual void setSizePercent(float widthPercent, float hwRatio);

    virtual void setWidthAuto();

    virtual void setHeightAuto();

    float widthPercent, hwRatio;

    virtual bool hasPercent();

#pragma mark TouchEvent

public:

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent);

    virtual bool onTouchEvent(TouchEvent *touchEvent);

    virtual void requestDisallowInterceptTouchEvent(bool disallowIntercept);

    virtual void setCustomTouchEventDispatcher(TouchEventDispatcher *touchEventDispatcher);

    /**
     * move事件超出view的范围时，如果要继续消费move事件返回true，比如MovingView，ProgressBar，其余View默认返回false
     * @return
     */
    virtual bool forceRequestTouchMove();

protected:

    std::unique_ptr<TouchEventDispatcher> touchEventDispatcher;

#pragma mark moving

public:

    float translateX = 0.0f;
    float translateY = 0.0f;

#pragma mark Animator

protected:

    std::unique_ptr<IAnimator> animator;

public:
    float animTranslateX = 0.0f;
    float animTranslateY = 0.0f;

protected:

    bool isDirty;

#pragma mark cakllbacks

public:

    virtual void setLayoutCallback(std::function<void(int, int, int, int)> callback);

    virtual void removeLayoutCallback();

    virtual void setOnClickListener(std::function<void(View *)> clickListener);

    virtual std::function<void(View *)> getClickListener();

    virtual void removeClickListener();

    virtual void performClick();

protected:

    std::function<void(int, int, int, int)> viewLayoutCallback = nullptr;

    std::function<void(View *)> viewClickListener = nullptr;

    std::shared_ptr<SkiaUIContext> context = nullptr;

};
