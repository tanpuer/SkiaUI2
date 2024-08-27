#pragma once

#include "FlexboxLayout.h"

/**
 * 如果是wrap 高度=子View高度之和，不支持嵌套ScrollView
 */
class ScrollView : public FlexboxLayout {

public:
    constexpr static int MIN_VELOCITY = 50;
    constexpr static int MAX_VELOCITY = 8000;
    constexpr static float FLING_FRICTION = 0.015f;
    static float DECELERATION_RATE;
    constexpr static float INFLEXION = 0.35f;
    constexpr static float GRAVITY = 9.8f;

public:

    ScrollView();

    virtual ~ScrollView();

    virtual void measure() override;

    virtual void layout(int l, int t, int r, int b) override;

    virtual void updateTranslateY(float diffY);

    virtual void setTranslateY(float y);

    virtual void updateTranslateX(float diffX);

    virtual void setFlexWrap(YGWrap wrap) override;

    virtual void setFlexDirection(YGFlexDirection direction) override;

    virtual bool addView(View *view) override;

    virtual bool addViewAt(View *view, uint32_t index) override;

    virtual bool removeView(View *view) override;

    virtual bool removeViewAt(uint32_t index) override;

    virtual bool canScroll();

    virtual void draw(SkCanvas *canvas) override;

    virtual bool ignoreChildDraw(const View *child);

    void startFling();

    void stopFling();

    void addScrollCallback(std::function<void(float dx, float dy)> callback);

    virtual const char *name() override;

    virtual void scrollToIndex(int index, bool animated);

    bool isScroller() override;

    /**
     * vertical : value < 0 down; value > 0 up
     * horizontal: value < 0 left; value > 0 right
     */
    virtual void scrollTo(float value);

    virtual void scrollBy(float value);

protected:

    float calculateFlingTranslate();

    bool isFling;

    long startTime;

    std::vector<std::function<void(float dx, float dy)>> scrollCallbacks;

    /**
     * 上次滑动是往下/往右
     */
    bool lastScrollDown = true;
    bool lastScrollRight = true;

};
