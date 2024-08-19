#pragma once

#include "View.h"
#include "vector"
#include "Velocity.h"

class ViewGroup : public View {

public:

    ViewGroup();

    virtual ~ViewGroup();

    const char *name() override;

    void measureChild(View *child);

    void setMeasuredDimension(int _measuredWidth, int _measuredHeight) override;

    /**
     * 子类ViewGroup必须复写
     */
    virtual void layout(int l, int t, int r, int b) override;

    virtual void draw(SkCanvas *canvas) override;

    bool isViewGroup() override;

    int getChildHeightSum();

    int getChildWidthSum();

    virtual YGConfigRef getConfig();

    virtual void onShow() override;

    virtual void onHide() override;

#pragma mark ViewGroup api

    virtual bool addView(View *view);

    virtual bool addViewAt(View *view, uint32_t index);

    virtual bool removeView(View *view);

    virtual bool removeViewAt(uint32_t index);

    virtual void removeAllViews();

    /**
     * 子视图在主轴上的排列对齐方式
     * @param justify
     */
    virtual void setJustifyContent(YGJustify justify);

    virtual const char *getJustifyContent();

    /**
     * 子视图在侧轴上的排列方式
     * @param align
     */
    virtual void setAlignItems(YGAlign align);

    virtual const char *getAlignItems();

    /**
     * 子视图行与行之间在侧轴上的对齐方式
     * @param align
     */
    virtual void setAlignContent(YGAlign align);

    /**
     * wrap类型
     * @param wrap 所有的子视图都会排列在一行之中
     * no_wrap 所有的子视图会从左到右，从上到下排列
     * wrap_reverse 从左到右，从下到上进行排列
     */
    virtual void setFlexWrap(YGWrap wrap);

    const char *getFLexWrap();

    /**
     * flex的方向
     * @param direction
     */
    virtual void setFlexDirection(YGFlexDirection direction);

    virtual const char *getFlexDirection();

    std::vector<View *> children;

    float maxChildWidth, maxChildHeight;

    bool hasPercent() override;

#pragma mark TouchEvent

    virtual bool dispatchTouchEvent(TouchEvent *touchEvent);

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    void requestDisallowInterceptTouchEvent(bool disallowIntercept) override;

    virtual bool dispatchVelocity(Velocity *velocity);

};
