//
// Created by cw on 1/24/22.
//

#ifndef SKIAUI_VIEW_H
#define SKIAUI_VIEW_H

#include <cstdint>
#include <yoga/Yoga.h>
#include <memory>
#include "array"
#include "memory"
#include "core/SkPaint.h"
#include "core/SkCanvas.h"
#include "core/SkRRect.h"
#include "LayoutParams.h"
#include "TouchEventDispatcher.h"
#include "IAnimator.h"
#include "effects/SkRuntimeEffect.h"

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

    /**
     * 任何组件都可以复写，计算完宽高后必须调用setMeasuredDimension，包括ViewGroup及其子类
     * @param widthMeasureSpec
     * @param heightMeasureSpec
     */
    virtual void measure(int widthMeasureSpec, int heightMeasureSpec);

    /**
     * 作为View/TextView/ImageView 这种组件必须明确指出自己的宽高，ViewGroup根据子View的宽高和parent-ViewGroup的限制来设置自己的宽高
     * @param _measuredWidth
     * @param _measuredHeight
     */
    virtual void setMeasuredDimension(int _measuredWidth, int _measuredHeight);

    virtual void layout(int l, int t, int r, int b);

    virtual void draw(SkCanvas *canvas);

    virtual bool isViewGroup();

    void setConfig(YGConfigRef config);

    /**
     * 在AlignItems的基础上自定义每个子视图的对齐方式
     * @param align
     */
    virtual void setAlignSelf(YGAlign align);

    YGNodeRef node;

    YGConfigRef config = nullptr;

protected:

    int width, height;

    int minWidth, minHeight;

protected:

    static int getDefaultSize(int minSize, int measureSpec);

#pragma mark LayoutParams相关
public:

    std::unique_ptr<LayoutParams> layoutParams;

    virtual void setLayoutParams(LayoutParams *layoutParams);

    virtual LayoutParams *getLayoutParams();

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

    SkPaint *paint;

    int marginLeft, marginTop, marginRight, marginBottom;

    SkIRect skRect;

    int cornerRadius;

    SkRect skRectWithBorder;

    virtual void setShaderSource(const char* data);

    virtual void setShaderPath(const char* path);

    sk_sp<SkRuntimeEffect> runtimeEffect;

public:
    //todo 后续才支持的
    virtual void setPadding(std::array<int, 4> paddings);

    virtual void setPadding(int padding);

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

    virtual void handleAnimation();

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

};

#endif //SKIAUI_VIEW_H
