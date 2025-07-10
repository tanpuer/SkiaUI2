#pragma once

#include <memory>

namespace HYSkiaUI {

class OverScroller;
class OverScrollerSpline {
public:
    OverScrollerSpline();
    enum class State { Spline, Cubic, Ballistic };

    OverScrollerSpline::State getState() const;
    void setFriction(float);
    void updateScroll(float);
    void startScroll(float start, float distance, float duration);
    void finish();
    bool isFinished() const;
    void setFinal(float);
    bool springBack(float start, float min, float max);
    void fling(float start, float end, float min, float max, float velocity);
    bool update();
    void reset();

private:
    float computeDeceleration(float velocity);
    void adjustDuration(float start, float oldFinal, float newFinal);
    void startSpringBack(float start, float end, float velocity);
    double getSplineDeceleration(float velocity);
    double getSplineFlingDistance(float velocity);
    float getSplineFlingDuration(float velocity);
    void fitOnBounceCurve(float start, float end, float velocity);
    void startBounceAfterEdge(float start, float end, float velocity);
    void startAfterEdge(float start, float end, float max, float velocity);
    void notifyEdgeReached(float start, float end, float overscrollLimit);
    void onEdgeReached();
    bool continueWhenFinished();

    float mStart = 0.0f;
    float mCurrent = 0.0f;
    float mFinal = 0.0f;
    float mDeceleration;
    float mPhysicalCoeff;
    float mPPi = 3.0f * 160.f;  // FIXME getDisplayMetrics().density * 160.0f;
    float mFlingFriction = 0.015f;
    float mVelocity = 0.0f;
    float mCurrVelocity = 0.0f;
    float mDuration = 0.0f;
    float mDistance = 0.0f;
    float mSplineDuration = 0.0f;
    float mSplineDistance = 0.0f;
    float mAllowedOverscroll = 0.0f;
    double mStartTime = 0.0;
    bool mFinished = true;
    State mState = State::Spline;

    friend class OverScroller;
};

class OverScroller {
public:
    enum class Mode {
        Scroller,  // scroller move
        Fling      // fling mode
    };

    // Create scroller with interpolator, the interpolator only using for scroll animation, fling don't need
    // @param[in] inter  User can set the interpolator, if null will using default.
    OverScroller();
    ~OverScroller();

    // Get the current/last scroller mode
    // @return Scroller mode (fling/scroller)
    OverScroller::Mode getMode() const;

    // Check current is finish animation
    // @return true finish
    bool isFinished() const;

    // Notifies the algorithm that an horizontal edge was reached
    // @param[in] start
    // @param[in] end
    // @param[in] overscroll
    void notifyHorizontalEdgeReached(float, float, float);

    // Notifies the algorithm that a vertical edge was reached
    // @param[in] start
    // @param[in] end
    // @param[in] overscroll
    void notifyVerticalEdgeReached(float, float, float);

    // Check current animation state for scroll X
    // @return State
    OverScrollerSpline::State getStateX() const;

    // Check current animation state for scroll Y
    // @return State
    OverScrollerSpline::State getStateY() const;

    // Get the start pos
    float getStartX() const;

    // Get the start pos
    float getStartY() const;

    // Get the current pos
    float getCurrentX() const;

    // Get the current pos
    float getCurrentY() const;

    // Get the final pos
    float getFinalX() const;

    // Get the final pos
    float getFinalY() const;

    // Get the current velocity
    float getCurrVelocity() const;

    // End the animation at the current position.
    void forceFinished(bool finished);

    // Abort the animation
    void abortAnimation();

    // Set the friction for fling animation
    // @param friction  default value is 0.015f
    void setFriction(float friction);

    // Computes the scroller new location.
    // @return True if NOT finished
    bool computeScrollOffset();

    // Start to scroll animation
    void startScroll(float startx, float starty, float dx, float dy, float duration);

    // Spring the scroller back to its closest edge after an overscroll
    // bool springBack(float startx, float starty, float minx, float miny, float maxx, float maxy);
    bool springBack(float startX, float startY, float minX, float maxX, float minY, float maxY);

    // Start fling animation
    void fling(float startx, float starty, float velocityx, float velocityy, float minx, float miny, float maxx,
               float maxy, float overx, float overy);

    // Whether the scroller is currently over scrolled.
    // @return bool
    bool isOverScrolled() const;

    // Time elapsed since beginning of scrolling animation.
    // @return Time in ms.
    double timeElapsed() const;

    // Update the current offset, one frame need to call one time
    // Internal will using 1000/60.f for frame time
    // @return false is finished the animation
    bool update();

    void reset();

    float getInterpolation(float progress);

private:
    void startSpringBack(float start, float end, float velocity);
    float computeDeceleration(float);
    float getSplineFlingDuration(float);
    double getSplineDeceleration(float);
    double getSplineFlingDistance(float);

private:
    OverScrollerSpline mScrollX;
    OverScrollerSpline mScrollY;
    Mode mMode = Mode::Scroller;
    bool mUseFlingMomentum = false;
};

}
