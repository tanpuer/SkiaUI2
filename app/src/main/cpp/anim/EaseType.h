#pragma once

#include <cstdint>
#include "math.h"

namespace HYSkiaUI {

// Easing functions: https://easings.net/en
enum class EaseType : uint8_t {
    Linear = 0,
    SineIn,
    SineOut,
    SineInOut,
    QuadIn,
    QuadOut,
    QuadInOut,
    CubicIn,
    CubicOut,
    CubicInOut,
    QuartIn,
    QuartOut,
    QuartInOut,
    QuintIn,
    QuintOut,
    QuintInOut,
    ExpoIn,
    ExpoOut,
    ExpoInOut,
    CircIn,
    CircOut,
    CircInOut,
    BackIn,
    BackOut,
    BackInOut,
    ElasticIn,
    ElasticOut,
    ElasticInOut,
    BounceIn,
    BounceOut,
    BounceInOut,
    Last
};

#define kPi      3.141592653 // 180 deg

// @param t, the progress that change
// @param b, will always be added to the result
// @param c, the result range
// @param d,
static float linearEase(float t, float b, float c, float d) {
    return c * t / d + b;
}

static float sineEaseIn(float t, float b, float c, float d) {
    return -c * cos(t / d * (kPi / 2)) + c + b;
}

static float sineEaseOut(float t, float b, float c, float d) {
    return c * sin(t / d * (kPi / 2)) + b;
}

static float sineEaseInOut(float t, float b, float c, float d) {
    return -c / 2 * (cos(kPi * t / d) - 1) + b;
}

static float quadEaseIn(float t, float b, float c, float d) {
    t /= d;
    return c * t * t + b;
}

static float quadEaseOut(float t, float b, float c, float d) {
    t /= d;
    return -c * t * (t - 2) + b;
}

static float quadEaseInOut(float t, float b, float c, float d) {
    if ((t /= (d / 2)) < 1)
        return ((c / 2) * (t * t)) + b;
    t--;
    return -c / 2 * (((t - 2) * t) - 1) + b;
}

static float cubicEaseIn(float t, float b, float c, float d) {
    t /= d;
    return c * t * t * t + b;
}

static float cubicEaseOut(float t, float b, float c, float d) {
    t = t / d - 1;
    return c * (t * t * t + 1) + b;
}

static float cubicEaseInOut(float t, float b, float c, float d) {
    if ((t /= d / 2) < 1)
        return c / 2 * t * t * t + b;
    t -= 2;
    return c / 2 * (t * t * t + 2) + b;
}

static float quartEaseIn(float t, float b, float c, float d) {
    t /= d;
    return c * t * t * t * t + b;
}

static float quartEaseOut(float t, float b, float c, float d) {
    t = t / d - 1;
    return -c * (t * t * t * t - 1) + b;
}

static float quartEaseInOut(float t, float b, float c, float d) {
    if ((t /= d / 2) < 1)
        return c / 2 * t * t * t * t + b;
    t -= 2;
    return -c / 2 * (t * t * t * t - 2) + b;
}

static float quintEaseIn(float t, float b, float c, float d) {
    t /= d;
    return c * t * t * t * t * t + b;
}

static float quintEaseOut(float t, float b, float c, float d) {
    t = t / d - 1;
    return c * (t * t * t * t * t + 1) + b;
}

static float quintEaseInOut(float t, float b, float c, float d) {
    if ((t /= d / 2) < 1)
        return c / 2 * t * t * t * t * t + b;
    t -= 2;
    return c / 2 * (t * t * t * t * t + 2) + b;
}

static float expoEaseIn(float t, float b, float c, float d) {
    return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

static float expoEaseOut(float t, float b, float c, float d) {
    return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

static float expoEaseInOut(float t, float b, float c, float d) {
    if (t == 0) return b;
    if (t == d) return b + c;
    if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
    return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}

static float circEaseIn(float t, float b, float c, float d) {
    t /= d;
    return -c * (sqrt(1 - t * t) - 1) + b;
}

static float circEaseOut(float t, float b, float c, float d) {
    t = t / d - 1;
    return c * sqrt(1 - t * t) + b;
}

static float circEaseInOut(float t, float b, float c, float d) {
    if ((t /= d / 2) < 1)
        return -c / 2 * (sqrt(1 - t * t) - 1) + b;
    t -= 2;
    return c / 2 * (sqrt(1 - t * t) + 1) + b;
}

static float backEaseIn(float t, float b, float c, float d) {
    float s = 1.70158f;
    float postFix = t /= d;
    return c * (postFix) * t * ((s + 1) * t - s) + b;
}

static float backEaseOut(float t, float b, float c, float d) {
    float s = 1.70158f;
    t = t / d - 1;
    return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

static float backEaseInOut(float t, float b, float c, float d) {
    float s = 1.70158f;
    if ((t /= d / 2) < 1) {
        s *= (1.525f);
        return c / 2 * (t * t * ((s + 1) * t - s)) + b;
    }
    float postFix = t -= 2;
    s *= (1.525f);
    return c / 2 * ((postFix) * t * ((s + 1) * t + s) + 2) + b;
}

static float elasticEaseIn(float t, float b, float c, float d) {
    if (t == 0) return b;
    if ((t /= d) == 1) return b + c;
    float p = d * .3f;
    float a = c;
    float s = p / 4;
    float postFix =
            a * pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
    return -(postFix * sin((t * d - s) * (2 * kPi) / p)) + b;
}

static float elasticEaseOut(float t, float b, float c, float d) {
    if (t == 0) return b;
    if ((t /= d) == 1) return b + c;
    float p = d * .3f;
    float a = c;
    float s = p / 4;
    return (a * pow(2, -10 * t) * sin((t * d - s) * (2 * kPi) / p) + c + b);
}

static float elasticEaseInOut(float t, float b, float c, float d) {
    if (t == 0) return b;
    if ((t /= d / 2) == 2) return b + c;
    float p = d * (.3f * 1.5f);
    float a = c;
    float s = p / 4;

    if (t < 1) {
        float postFix = a * pow(2, 10 * (t -= 1)); // postIncrement is evil
        return -.5f * (postFix * sin((t * d - s) * (2 * kPi) / p)) + b;
    }
    float postFix = a * pow(2, -10 * (t -= 1)); // postIncrement is evil
    return postFix * sin((t * d - s) * (2 * kPi) / p) * .5f + c + b;
}

static float bounceEaseOut(float t, float b, float c, float d) {
    if ((t /= d) < (1 / 2.75f)) {
        return c * (7.5625f * t * t) + b;
    } else if (t < (2 / 2.75f)) {
        float postFix = t -= (1.5f / 2.75f);
        return c * (7.5625f * (postFix) * t + .75f) + b;
    } else if (t < (2.5 / 2.75)) {
        float postFix = t -= (2.25f / 2.75f);
        return c * (7.5625f * (postFix) * t + .9375f) + b;
    } else {
        float postFix = t -= (2.625f / 2.75f);
        return c * (7.5625f * (postFix) * t + .984375f) + b;
    }
}

static float bounceEaseIn(float t, float b, float c, float d) {
    return c - bounceEaseOut(d - t, 0, c, d) + b;
}

static float bounceEaseInOut(float t, float b, float c, float d) {
    if (t < d / 2) return bounceEaseIn(t * 2, 0, c, d) * .5f + b;
    else return bounceEaseOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}

typedef float EaseFunc(float t, float b, float c, float d);

static EaseFunc *sEaseLst[(uint32_t) EaseType::Last] = {
        linearEase,       //! Linear
        sineEaseIn,       //! SineIn
        sineEaseOut,      //! SineOut
        sineEaseInOut,    //! SineInOut
        quadEaseIn,       //! QuadIn
        quadEaseOut,      //! QuadOut
        quadEaseInOut,    //! QuadInOut
        cubicEaseIn,      //! CubicIn
        cubicEaseOut,     //! CubicOut
        cubicEaseInOut,   //! CubicInOut
        quartEaseIn,      //! QuartIn
        quartEaseOut,     //! QuartOut
        quartEaseInOut,   //! QuartInOut
        quintEaseIn,      //! QuintIn
        quintEaseOut,     //! QuintOut
        quintEaseInOut,   //! QuintInOut
        expoEaseIn,       //! ExpoIn
        expoEaseOut,      //! ExpoOut
        expoEaseInOut,    //! ExpoInOut
        circEaseIn,       //! CircIn
        circEaseOut,      //! CircOut
        circEaseInOut,    //! CircInOut
        backEaseIn,       //! BackIn
        backEaseOut,      //! BackOut
        backEaseInOut,    //! BackInOut
        elasticEaseIn,    //! ElasticIn
        elasticEaseOut,   //! ElasticOut
        elasticEaseInOut, //! ElasticInOut
        bounceEaseIn,     //! BounceIn
        bounceEaseOut,    //! BounceOut
        bounceEaseInOut   //! BounceInOut
};

}
