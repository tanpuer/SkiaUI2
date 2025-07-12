#include "touch/OverScroller.h"

#include <algorithm>
#include <chrono>
#include <cmath>

namespace HYSkiaUI {

static const float DECELERATION_RATE = std::log(0.78f) / std::log(0.9f);
static const float INFLEXION = 0.35f;  // Tension lines cross at (INFLEXION, 1)
static const float START_TENSION = 0.5f;
static const float END_TENSION = 1.0f;
static const float P1 = START_TENSION * INFLEXION;
static const float P2 = 1.0f - END_TENSION * (1.0f - INFLEXION);
static const int NB_SAMPLES = 200;
static float SPLINE_POSITION[NB_SAMPLES + 1];
static float SPLINE_TIME[NB_SAMPLES + 1];

static double msNowApi() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
}

template <typename T>
inline int Sign(const T val) {
    return (static_cast<T>(0) <= val) - (val < static_cast<T>(0));
}

static bool sInitSpline = false;
static void initSpline() {
    if (sInitSpline) return;

    float x_min = 0.0f;
    float y_min = 0.0f;
    for (int i = 0; i < NB_SAMPLES; i++) {
        float alpha = (float)i / NB_SAMPLES;
        float x_max = 1.0f;
        float x, tx, coef;
        while (true) {
            x = x_min + (x_max - x_min) / 2.0f;
            coef = 3.0f * x * (1.0f - x);
            tx = coef * ((1.0f - x) * P1 + x * P2) + x * x * x;
            if (std::abs(tx - alpha) < 1E-5) break;
            if (tx > alpha)
                x_max = x;
            else
                x_min = x;
        }
        SPLINE_POSITION[i] = coef * ((1.0f - x) * START_TENSION + x) + x * x * x;
        float y_max = 1.0f;
        float y, dy;
        while (true) {
            y = y_min + (y_max - y_min) / 2.0f;
            coef = 3.0f * y * (1.0f - y);
            dy = coef * ((1.0f - y) * START_TENSION + y) + y * y * y;
            if (std::abs(dy - alpha) < 1E-5) break;
            if (dy > alpha)
                y_max = y;
            else
                y_min = y;
        }
        SPLINE_TIME[i] = coef * ((1.0f - y) * P1 + y * P2) + y * y * y;
    }
    SPLINE_POSITION[NB_SAMPLES] = SPLINE_TIME[NB_SAMPLES] = 1.0f;
}

static float GRAVITY = 8000.0f;
static float getDeceleration(int velocity) { return velocity > 0 ? -GRAVITY : GRAVITY; }

OverScrollerSpline::OverScrollerSpline() {
    initSpline();

    mDeceleration = computeDeceleration(mFlingFriction);  // default scroll friction
    mPhysicalCoeff = computeDeceleration(0.84f);          // look and feel tuning
}

OverScrollerSpline::State OverScrollerSpline::getState() const { return mState; }

void OverScrollerSpline::finish() {
    mCurrent = mFinal;
    mFinished = true;
}

bool OverScrollerSpline::isFinished() const { return mFinished; }

void OverScrollerSpline::setFinal(float f) {
    mFinal = f;
    mFinished = false;
}

void OverScrollerSpline::setFriction(float friction) {
    mDeceleration = computeDeceleration(friction);
    mFlingFriction = friction;
}

float OverScrollerSpline::computeDeceleration(float friction) {
    return 9.807     // g (m/s^2)
           * 39.37f  // inch/meter
           * mPPi    // pixels per inch
           * friction;
}

bool OverScrollerSpline::springBack(float start, float min, float max) {
    mFinished = true;
    mCurrent = mStart = mFinal = start;
    mVelocity = 0;
    mStartTime = msNowApi();
    mDuration = 0;
    if (start < min) {
        startSpringBack(start, min, 0);
    } else if (start > max) {
        startSpringBack(start, max, 0);
    }
    return !mFinished;
}

void OverScrollerSpline::startSpringBack(float start, float end, float velocity) {
    mFinished = false;
    mState = State::Cubic;
    mCurrent = mStart = start;
    mFinal = end;
    const float delta = start - end;
    mDeceleration = getDeceleration(delta);
    // TODO take velocity into account
    mVelocity = -delta;  // only sign is used
    mAllowedOverscroll = std::abs(delta);
    mDuration = static_cast<float>(1000.0 * std::sqrt(-2.0 * delta / mDeceleration));
}

void OverScrollerSpline::adjustDuration(float start, float oldFinal, float newFinal) {
    const float oldDistance = oldFinal - start;
    const float newDistance = newFinal - start;
    const float x = std::abs(newDistance / oldDistance);
    const int index = static_cast<int>(NB_SAMPLES * x);
    if (index < NB_SAMPLES) {
        const float x_inf = (float)index / NB_SAMPLES;
        const float x_sup = (float)(index + 1) / NB_SAMPLES;
        const float t_inf = SPLINE_TIME[index];
        const float t_sup = SPLINE_TIME[index + 1];
        const float timeCoef = t_inf + (x - x_inf) / (x_sup - x_inf) * (t_sup - t_inf);
        mDuration *= timeCoef;
    }
}

void OverScrollerSpline::startScroll(float start, float distance, float duration) {
    mFinished = false;
    mCurrent = mStart = start;
    mFinal = start + distance;
    mStartTime = msNowApi();
    mDuration = duration;
    mDeceleration = 0.0f;
    mVelocity = 0;
}

void OverScrollerSpline::fling(float start, float velocity, float min, float max, float over) {
    mAllowedOverscroll = over;
    mFinished = false;
    mCurrVelocity = mVelocity = velocity;
    mDuration = mSplineDuration = 0;
    mStartTime = msNowApi();
    mCurrent = mStart = start;
    if (start > max || start < min) {
        startAfterEdge(start, min, max, velocity);
        return;
    }
    mState = State::Spline;
    double totalDistance = 0.0;
    if (velocity != 0) {
        mDuration = mSplineDuration = getSplineFlingDuration(velocity);
        totalDistance = getSplineFlingDistance(velocity);
    }
    mSplineDistance = static_cast<float>(totalDistance * Sign(velocity));
    mFinal = start + mSplineDistance;
    // Clamp to a valid final position
    if (mFinal < min) {
        adjustDuration(mStart, mFinal, min);
        mFinal = min;
    }
    if (mFinal > max) {
        adjustDuration(mStart, mFinal, max);
        mFinal = max;
    }
}

float OverScrollerSpline::getSplineFlingDuration(float velocity) {
    double l = getSplineDeceleration(velocity);
    double decelMinusOne = DECELERATION_RATE - 1.0;
    return (1000.f * std::exp(l / decelMinusOne));
}

double OverScrollerSpline::getSplineDeceleration(float velocity) {
    return std::log(INFLEXION * std::abs(velocity) / (mFlingFriction * mPhysicalCoeff));
}

double OverScrollerSpline::getSplineFlingDistance(float velocity) {
    double l = getSplineDeceleration(velocity);
    double decelMinusOne = DECELERATION_RATE - 1.0;
    return mFlingFriction * mPhysicalCoeff * std::exp(DECELERATION_RATE / decelMinusOne * l);
}

void OverScrollerSpline::fitOnBounceCurve(float start, float end, float velocity) {
    // Simulate a bounce that started from edge
    const float durationToApex = -velocity / mDeceleration;
    // The float cast below is necessary to avoid integer overflow.
    const float velocitySquared = static_cast<float>(velocity * velocity);
    const float distanceToApex = velocitySquared / 2.0f / std::abs(mDeceleration);
    const float distanceToEdge = std::abs(end - start);
    const float totalDuration =
        static_cast<float>(std::sqrt(2.0 * (distanceToApex + distanceToEdge) / std::abs(mDeceleration)));
    const float durationOffset = static_cast<float>(1000.0f * (totalDuration - durationToApex));
    mStartTime -= durationOffset;
    mCurrent = mStart = end;
    mVelocity = static_cast<float>(-mDeceleration * totalDuration);
}

void OverScrollerSpline::startBounceAfterEdge(float start, float end, float velocity) {
    mDeceleration = getDeceleration(velocity == 0 ? start - end : velocity);
    fitOnBounceCurve(start, end, velocity);
    onEdgeReached();
}

void OverScrollerSpline::startAfterEdge(float start, float min, float max, float velocity) {
    if (start > min && start < max) {
        mFinished = true;
        return;
    }
    const bool positive = start > max;
    const float edge = positive ? max : min;
    const float overDistance = start - edge;
    bool keepIncreasing = (overDistance * velocity) >= 0;
    if (keepIncreasing) {
        // Will result in a bounce or a to_boundary depending on velocity.
        startBounceAfterEdge(start, edge, velocity);
    } else {
        const double totalDistance = getSplineFlingDistance(velocity);
        if (totalDistance > std::abs(overDistance)) {
            fling(start, velocity, positive ? min : start, positive ? start : max, mAllowedOverscroll);
        } else {
            startSpringBack(start, edge, velocity);
        }
    }
}

void OverScroller::notifyHorizontalEdgeReached(float startX, float finalX, float overX) {
    mScrollX.notifyEdgeReached(startX, finalX, overX);
}

void OverScroller::notifyVerticalEdgeReached(float startY, float finalY, float overY) {
    mScrollY.notifyEdgeReached(startY, finalY, overY);
}

void OverScrollerSpline::notifyEdgeReached(float start, float end, float over) {
    // mState is used to detect successive notifications
    if (mState == State::Spline) {
        mAllowedOverscroll = over;
        mStartTime = msNowApi();
        // We were in fling/scroll mode before: current velocity is such that distance to
        // edge is increasing. This ensures that startAfterEdge will not start a new fling.
        startAfterEdge(start, end, end, mCurrVelocity);
    }
}

void OverScrollerSpline::onEdgeReached() {
    // mStart, mVelocity and mStartTime were adjusted to their values when edge was reached.
    // The float cast below is necessary to avoid integer overflow.
    const float velocitySquared = mVelocity * mVelocity;
    float distance = velocitySquared / (2.0f * std::abs(mDeceleration));
    const float sign = Sign(mVelocity);
    if (distance > mAllowedOverscroll) {
        // Default deceleration is not sufficient to slow us down before boundary
        mDeceleration = -sign * velocitySquared / (2.0f * mAllowedOverscroll);
        distance = mAllowedOverscroll;
    }
    mAllowedOverscroll = distance;
    mState = State::Ballistic;
    mFinal = mStart + (mVelocity > 0 ? distance : -distance);
    mDuration = -(1000.0f * mVelocity / mDeceleration);
}

bool OverScrollerSpline::continueWhenFinished() {
    switch (mState) {
        case State::Spline:
            // Duration from start to null velocity
            if (mDuration < mSplineDuration) {
                // If the animation was clamped, we reached the edge
                mCurrent = mStart = mFinal;
                // TODO Better compute speed when edge was reached
                mVelocity = (int)mCurrVelocity;
                mDeceleration = getDeceleration(mVelocity);
                mStartTime += mDuration;
                onEdgeReached();
            } else {
                // Normal stop, no need to continue
                return false;
            }
            break;
        case State::Ballistic:
            mStartTime += mDuration;
            startSpringBack(mFinal, mStart, 0);
            break;
        case State::Cubic:
            return false;
    }

    update();
    return true;
}

bool OverScrollerSpline::update() {
    const double currentTime = msNowApi() - mStartTime;
    if (currentTime == 0) {
        // Skip work but report that we're still going if we have a nonzero duration.
        return mDuration > 0;
    }
    if (currentTime > mDuration) {
        return false;
    }
    double distance = 0.0;
    switch (mState) {
        case State::Spline: {
            const float t = (float)currentTime / mSplineDuration;
            const int index = (int)(NB_SAMPLES * t);
            float distanceCoef = 1.f;
            float velocityCoef = 0.f;
            if (index < NB_SAMPLES) {
                const float t_inf = (float)index / NB_SAMPLES;
                const float t_sup = (float)(index + 1) / NB_SAMPLES;
                const float d_inf = SPLINE_POSITION[index];
                const float d_sup = SPLINE_POSITION[index + 1];
                velocityCoef = (d_sup - d_inf) / (t_sup - t_inf);
                distanceCoef = d_inf + (t - t_inf) * velocityCoef;
            }
            distance = distanceCoef * mSplineDistance;
            mCurrVelocity = velocityCoef * mSplineDistance / mSplineDuration * 1000.0f;
            break;
        }
        case State::Ballistic: {
            const float t = currentTime / 1000.0f;
            mCurrVelocity = mVelocity + mDeceleration * t;
            distance = mVelocity * t + mDeceleration * t * t / 2.0f;
            break;
        }
        case State::Cubic: {
            const float t = (float)(currentTime) / mDuration;
            const float t2 = t * t;
            const float sign = Sign(mVelocity);
            distance = sign * mAllowedOverscroll * (3.0f * t2 - 2.0f * t * t2);
            mCurrVelocity = sign * mAllowedOverscroll * 6.0f * (-t + t2);
            break;
        }
    }
    mCurrent = mStart + std::round(distance);
    return true;
}

void OverScrollerSpline::reset() {
    mStart = 0.0f;
    mCurrent = 0.0f;
    mFinal = 0.0f;
    mVelocity = 0.0f;
    mCurrVelocity = 0.0f;
    mDuration = 0.0f;
    mDistance = 0.0f;
    mSplineDuration = 0.0f;
    mSplineDistance = 0.0f;
    mAllowedOverscroll = 0.0f;
    mStartTime = 0.0;
    mFinished = true;
}

OverScroller::OverScroller() {}

OverScroller::~OverScroller() {}

float OverScroller::getCurrentX() const { return mScrollX.mCurrent; }

float OverScroller::getCurrentY() const { return mScrollY.mCurrent; }

float OverScroller::getFinalX() const { return mScrollX.mFinal; }

float OverScroller::getFinalY() const { return mScrollY.mFinal; }

void OverScroller::forceFinished(bool finish) { mScrollX.mFinished = mScrollY.mFinished = finish; }

void OverScroller::abortAnimation() {
    mScrollX.finish();
    mScrollY.finish();
}

void OverScroller::setFriction(float friction) {
    mScrollX.setFriction(friction);
    mScrollY.setFriction(friction);
}

OverScroller::Mode OverScroller::getMode() const { return mMode; }

bool OverScroller::isFinished() const { return mScrollX.isFinished() && mScrollY.isFinished(); }

OverScrollerSpline::State OverScroller::getStateX() const { return mScrollX.getState(); }

OverScrollerSpline::State OverScroller::getStateY() const { return mScrollY.getState(); }

bool OverScroller::computeScrollOffset() {
    if (isFinished()) {
        return false;
    }
    const double time = msNowApi();
    const double elapsedTime = time - mScrollX.mStartTime;
    switch (mMode) {
        case Mode::Scroller: {
            const float duration = mScrollX.mDuration;
            if (elapsedTime < duration) {
                const float q = getInterpolation(elapsedTime / duration);
                mScrollX.updateScroll(q);
                mScrollY.updateScroll(q);
            } else {
                abortAnimation();
            }
            break;
        }
        case Mode::Fling: {
            if (!mScrollX.mFinished) {
                if (!mScrollX.update()) {
                    if (!mScrollX.continueWhenFinished()) {
                        mScrollX.finish();
                    }
                }
            }
            if (!mScrollY.mFinished) {
                if (!mScrollY.update()) {
                    if (!mScrollY.continueWhenFinished()) {
                        mScrollY.finish();
                    }
                }
            }
            break;
        }
    }
    return true;
}

void OverScrollerSpline::updateScroll(float q) {
    // TODO:
    // Consider doing subpixel rounding here to allow for smoother
    // animation on bigger screens (2-digits floating-point rounding, eg. 2.017 -> 2.02)
    // while maintaining a certain level of sanitization (currently doing the same with Yoga)
    //
    // NOTE:
    // Sanitization is important to ensure that floating-point computation stays within expectations
    // internally and for end-users, by providing "sane" outputs that can be safely compared against...
    mCurrent = mStart + std::round(q * (mFinal - mStart));
}

void OverScroller::startScroll(float startx, float starty, float dx, float dy, float duration) {
    mMode = Mode::Scroller;
    mScrollX.startScroll(startx, dx, duration);
    mScrollY.startScroll(starty, dy, duration);
}

bool OverScroller::springBack(float startX, float startY, float minX, float maxX, float minY, float maxY) {
    mMode = Mode::Fling;
    const bool springBackX = mScrollX.springBack(startX, minX, maxX);
    const bool springBackY = mScrollY.springBack(startY, minY, maxY);
    return springBackX || springBackY;
}

void OverScroller::fling(float startx, float starty, float velocityx, float velocityy, float minx, float miny,
                         float maxx, float maxy, float overx, float overy) {
    // Continue a scroll or fling in progress
    if (mUseFlingMomentum && !isFinished()) {
        float oldVelocityX = mScrollX.mCurrVelocity;
        float oldVelocityY = mScrollY.mCurrVelocity;
        if (Sign(velocityx) == Sign(oldVelocityX) && Sign(velocityx) == Sign(oldVelocityY)) {
            velocityx += oldVelocityX;
            velocityy += oldVelocityY;
        }
    }
    mMode = Mode::Fling;
    mScrollX.fling(startx, velocityx, minx, maxx, overx);
    mScrollY.fling(starty, velocityy, miny, maxy, overy);
}

bool OverScroller::isOverScrolled() const {
    return ((!mScrollX.mFinished && mScrollX.mState != OverScrollerSpline::State::Spline) ||
            (!mScrollY.mFinished && mScrollY.mState != OverScrollerSpline::State::Spline));
}

double OverScroller::timeElapsed() const {
    const double time = msNowApi();
    const double startTime = std::min(mScrollX.mStartTime, mScrollY.mStartTime);
    return time - startTime;
}

void OverScroller::reset() {
    mScrollX.reset();
    mScrollY.reset();
}

float OverScroller::getInterpolation(float progress) {
    if (progress > 1.0f) {
        return 1.0f;
    } else if (progress < 0.0f) {
        return 0.f;
    }
    return progress;
}

}
