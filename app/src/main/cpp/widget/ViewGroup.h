//
// Created by cw on 1/24/22.
//

#ifndef SKIAUI_VIEWGROUP_H
#define SKIAUI_VIEWGROUP_H


#include "View.h"
#include "vector"
#include "LayoutParams.h"

class ViewGroup : public View {

public:

    ViewGroup();

    virtual ~ViewGroup();

    const char *name() override;

    void measureChild(View *child, int parentWidthMeasureSpec, int parentHeightMeasureSpec);

    int getChildMeasureSpec(View *child, int parentMeasureSpec, int padding, int childDimension);

    void setMeasuredDimension(int _measuredWidth, int _measuredHeight) override;

    /**
     * 子类ViewGroup必须复写
     */
    virtual void layout(int l, int t, int r, int b) override = 0;

    virtual void draw(SkCanvas *canvas) override;

    bool isViewGroup() override;

    virtual int getWidth() override;

    virtual int getHeight() override;

    int getMaxHeightInChildren();

    int getMaxWidthInChildren();

    int getChildHeightSum();

    int getChildWidthSum();

    virtual YGConfigRef getConfig();

#pragma mark ViewGroup api

    virtual bool addView(View *view, LayoutParams *layoutParams);

    virtual bool addViewAt(View *view, LayoutParams *layoutParams, uint32_t index);

    virtual bool removeView(View *view);

    virtual bool removeViewAt(uint32_t index);

    virtual void removeAllViews();

    /**
     * 子视图在主轴上的排列对齐方式
     * @param justify
     */
    virtual void setJustifyContent(YGJustify justify);

    /**
     * 子视图在侧轴上的排列方式
     * @param align
     */
    virtual void setAlignItems(YGAlign align);

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

    /**
     * flex的方向
     * @param direction
     */
    virtual void setFlexDirection(YGFlexDirection direction);

    std::vector<View *> children;

    float maxChildWidth, maxChildHeight;

    bool hasPercent();

#pragma mark TouchEvent

    virtual bool dispatchTouchEvent(TouchEvent *touchEvent);

    virtual bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    void requestDisallowInterceptTouchEvent(bool disallowIntercept) override;

};


#endif //SKIAUI_VIEWGROUP_H
