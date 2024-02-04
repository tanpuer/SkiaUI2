//
// Created by cw on 2022/5/24.
//

#ifndef SKIAUI_FLEXBOXLAYOUT_H
#define SKIAUI_FLEXBOXLAYOUT_H


#include "ViewGroup.h"

/**
 * 本来模仿Android，实现FrameLayout,LinearLayout，ConstraintLayout实现过于复杂，最终还是简单用一个FlexboxLayout来实现，毕竟Yoga就是基于Flexbox的布局引擎。
 */
class FlexboxLayout : public ViewGroup {

public:

    FlexboxLayout();

    ~FlexboxLayout();

    const char * name() override;

    virtual void setFlexDirection(YGFlexDirection direction) override;

    void measure(int widthMeasureSpec, int heightMeasureSpec) override;

    virtual void layout(int l, int t, int r, int b) override;

    virtual void setFlexWrap(YGWrap wrap) override;

    virtual bool addView(View *view, LayoutParams *layoutParams) override;

    virtual bool removeView(View *view) override;

    YGFlexDirection _direction = YGFlexDirectionColumn;

protected:

    void layoutVertical(int l, int t, int r, int b);

    void layoutHorizontal(int l, int t, int r, int b);

};


#endif //SKIAUI_FLEXBOXLAYOUT_H
