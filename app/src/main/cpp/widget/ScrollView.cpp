#include <ScrollDispatcher.h>
#include "ScrollView.h"
#include "algorithm"

float ScrollView::DECELERATION_RATE = (float) (log(0.78) / log(0.9));

ScrollView::ScrollView() : xVelocity(0.0f), yVelocity(0.0f), isFling(false),
                           startTime(0L) {
    touchEventDispatcher = std::make_unique<ScrollDispatcher>(this);
    scrollCallbacks = std::vector<std::function<void(float dx, float dy)>>();
}

ScrollView::~ScrollView() {
    scrollCallbacks.clear();
}

void ScrollView::measure() {
    for (auto &child: children) {
        measureChild(child);
    }
}

void ScrollView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (isFling) {
        calculateFlingTranslate();
    }
    if (_direction == YGFlexDirectionRow) {
        layoutHorizontal(l + translateX, t, r, b);
    } else {
        layoutVertical(l, t + translateY, r, b);
    }
}

void ScrollView::updateTranslateY(float diffY) {
    translateY += diffY;
    //可滑动的上限和下限
    auto maxTranslate = height - getChildHeightSum();
    if (translateY <= maxTranslate) {
        translateY = maxTranslate;
    }
    if (translateY >= 0) {
        translateY = 0;
    }
    for (auto &callback: scrollCallbacks) {
        callback(0.0, diffY);
    }
    lastScrollDown = diffY < 0.0f;
}

void ScrollView::setFlexWrap(YGWrap wrap) {
    assert(wrap == YGWrapNoWrap);
    FlexboxLayout::setFlexWrap(wrap);
}

void ScrollView::setFlexDirection(YGFlexDirection direction) {
    FlexboxLayout::setFlexDirection(direction);
}

void ScrollView::updateTranslateX(float diffX) {
    translateX += diffX;
    auto maxTranslate = width - getChildWidthSum();
    if (translateX <= maxTranslate) {
        translateX = maxTranslate;
    }
    if (translateX >= 0) {
        translateX = 0;
    }
    for (auto &callback: scrollCallbacks) {
        callback(diffX, 0.0);
    }
    lastScrollRight = diffX < 0.0f;
}

bool ScrollView::addView(View *view) {
    isDirty = true;
    auto index = YGNodeGetChildCount(node);
//    ALOGD("RecyclerView addView at %d %ld", index, children.size())
    return FlexboxLayout::addViewAt(view, index);
}

bool ScrollView::addViewAt(View *view, uint32_t index) {
    isDirty = true;
    return ViewGroup::addViewAt(view, index);
}

bool ScrollView::removeView(View *view) {
    isDirty = true;
    return FlexboxLayout::removeView(view);
}

bool ScrollView::removeViewAt(uint32_t index) {
    isDirty = true;
    return ViewGroup::removeViewAt(index);
}

bool ScrollView::canScroll() {
    if (_direction == YGFlexDirectionRow) {
        return abs(translateX) <= getChildWidthSum() - width;
    } else {
        return abs(translateY) <= getChildHeightSum() - height;
    }
}

void ScrollView::setVelocity(float x, float y) {
    this->xVelocity = x;
    this->yVelocity = y;
//    ALOGD("ScrollView setVelocity %f %f", x, y)
}

void ScrollView::startFling() {
    if (_direction == YGFlexDirectionColumn) {
        if (abs(yVelocity) <= MIN_VELOCITY) {
            yVelocity = .0f;
            return;
        } else if (abs(yVelocity) >= MAX_VELOCITY) {
            yVelocity = yVelocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
        }
        startTime = IAnimator::currTime;
        isFling = true;
    } else {
        if (abs(xVelocity) <= MIN_VELOCITY) {
            xVelocity = .0f;
            return;
        } else if (abs(xVelocity) >= MAX_VELOCITY) {
            xVelocity = xVelocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
        }
        startTime = IAnimator::currTime;
        isFling = true;
    }
}

void ScrollView::stopFling() {
    isFling = false;
}

float ScrollView::calculateFlingTranslate() {
    float velocity = yVelocity - (yVelocity > 0 ? 1.0f : -1.0f) * GRAVITY *
                                 (IAnimator::currTime - startTime); //v' = v + gt;
//    ALOGD("ScrollView velocity %f %f", yVelocity, velocity)
    if (yVelocity / velocity <= 0 || abs(velocity) <= MIN_VELOCITY) {
        yVelocity = .0f;
        isFling = false;
    }
//    float ppi = context.getResources().getDisplayMetrics().density * 160.0f;
//  see OverScroller.java
    float ppi = 3 * 160.0f;
    auto mPhysicalCoeff = GRAVITY * 39.37f * ppi * 0.84f;
    auto l = log(INFLEXION * abs(velocity) / (FLING_FRICTION * mPhysicalCoeff));
    double decelMinusOne = DECELERATION_RATE - 1.0;
    auto diff = FLING_FRICTION * mPhysicalCoeff * exp(DECELERATION_RATE / decelMinusOne * l);
    updateTranslateY(diff * (yVelocity > 0 ? 1.0 : -1.0) / 10.0);
    return 0.0f;
}

void ScrollView::addScrollCallback(std::function<void(float, float)> callback) {
    scrollCallbacks.emplace_back(callback);
}

void ScrollView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    for (auto child: children) {
        if (ignoreChildDraw(child)) {
//            ALOGD("ScrollView draw ignore invisible child %s %d %d", child->name(),
//                  child->skRect.top(), child->skRect.bottom())
        } else {
            child->draw(canvas);
        }
    }
}

bool ScrollView::ignoreChildDraw(const View *child) {
    if (_direction == YGFlexDirectionColumn) {
        const auto childRect = child->skRect;
        auto childTop = childRect.top();
        auto childBottom = childRect.bottom();
        auto scrollTop = skRect.top();
        auto scrollBottom = skRect.bottom();
        //bottom小于ScrollView的top-100/top大于ScrollView的bottom+100，则认为在屏幕上不可见，不进行绘制
        return childTop > scrollBottom + 100 || childBottom < scrollTop - 100;
    } else {
        const auto childRect = child->skRect;
        auto childLeft = childRect.left();
        auto childRight = childRect.right();
        auto scrollLeft = skRect.left();
        auto scrollRight = skRect.right();
        return childLeft > scrollRight + 100 || childRight < scrollLeft - 100;
    }
}

const char *ScrollView::name() {
    return "ScrollView";
}
