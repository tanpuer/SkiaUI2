#pragma once

#include "FlexboxLayout.h"
#include "OverScroller.h"

namespace HYSkiaUI {

class ScrollView : public FlexboxLayout {

public:
    constexpr static int MIN_VELOCITY = 50;
    constexpr static int MAX_VELOCITY = 18000;
    constexpr static int SCROLL_SLOP = 1;

public:

    ScrollView();

    ~ScrollView() override;

    virtual void measure() override;

    virtual void layout(int l, int t, int r, int b) override;

    virtual void updateTranslateY(float diffY);

    virtual void setTranslateY(float y) override;

    virtual void updateTranslateX(float diffX);

    virtual void setTranslateX(float x) override;

    virtual void setFlexWrap(YGWrap wrap) override;

    virtual void setFlexDirection(YGFlexDirection direction) override;

    virtual void draw(SkCanvas *canvas) override;

    virtual bool ignoreChildDraw(View *child);

    virtual void fling();

    void stopFling();

    virtual const char *name() override;

    virtual void scrollToIndex(int index, bool animated);

    bool isScroller() override;

    /**
     * vertical : value < 0 down; value > 0 up
     * horizontal: value < 0 left; value > 0 right
     */
    virtual void scrollTo(float value);

    virtual void scrollBy(float value);

    int getDistanceByIndex(int index);

    int getChildWidthSum() override;

    int getChildHeightSum() override;

protected:

    std::unique_ptr<OverScroller> mScroller;

    void adjustVelocity(float& velocity);

    float applyDamping(float offset, float maxDistance);

    int childrenHeightSum = 0;

    int childrenWidthSum = 0;

    float clampX = 0.0f;

    float clampY = 0.0f;

#pragma mark for RV

public:

    void setScrollEnd(bool flag);

protected:

    bool lastScrollEnd = true;

};

}
