#include <ScrollDispatcher.h>
#include "ScrollView.h"
#include "algorithm"
#include "LinearAnimator.h"

float ScrollView::DECELERATION_RATE = (float) (log(0.78) / log(0.9));

ScrollView::ScrollView() : isFling(false), startTime(0L) {
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
        updateTranslateX(0.0);
        layoutHorizontal(l + translateX, t, r, b);
    } else {
        updateTranslateY(0.0);
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

void ScrollView::setTranslateY(float y) {
    translateY = y;
    //可滑动的上限和下限
    auto maxTranslate = height - getChildHeightSum();
    if (translateY <= maxTranslate) {
        translateY = maxTranslate;
    }
    if (translateY >= 0) {
        translateY = 0;
    }
    lastScrollDown = y < 0.0f;
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
    markDirty();
    auto index = YGNodeGetChildCount(node);
//    ALOGD("RecyclerView addView at %d %ld", index, children.size())
    return FlexboxLayout::addViewAt(view, index);
}

bool ScrollView::addViewAt(View *view, uint32_t index) {
    markDirty();
    return ViewGroup::addViewAt(view, index);
}

bool ScrollView::removeView(View *view) {
    markDirty();
    return FlexboxLayout::removeView(view);
}

bool ScrollView::removeViewAt(uint32_t index) {
    markDirty();
    return ViewGroup::removeViewAt(index);
}

bool ScrollView::canScroll() {
    if (_direction == YGFlexDirectionRow) {
        return abs(translateX) <= getChildWidthSum() - width;
    } else {
        return abs(translateY) <= getChildHeightSum() - height;
    }
}

void ScrollView::startFling() {
    if (_direction == YGFlexDirectionColumn) {
        if (abs(yVelocity) <= MIN_VELOCITY) {
            yVelocity = .0f;
            onFlingStopped();
            return;
        } else if (abs(yVelocity) >= MAX_VELOCITY) {
            yVelocity = yVelocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
        }
        startTime = IAnimator::currTime;
        isFling = true;
    } else {
        if (abs(xVelocity) <= MIN_VELOCITY) {
            xVelocity = .0f;
            onFlingStopped();
            return;
        } else if (abs(xVelocity) >= MAX_VELOCITY) {
            xVelocity = xVelocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
        }
        startTime = IAnimator::currTime;
        isFling = true;
    }
}

void ScrollView::stopFling() {
    if (isFling) {
        isFling = false;
        onFlingStopped();
    }
}

float ScrollView::calculateFlingTranslate() {
    float velocity = yVelocity - (yVelocity > 0 ? 1.0f : -1.0f) * GRAVITY *
                                 (IAnimator::currTime - startTime); //v' = v + gt;
//    ALOGD("ScrollView velocity %f %f", yVelocity, velocity)
    if (yVelocity / velocity <= 0 || abs(velocity) <= MIN_VELOCITY) {
        yVelocity = .0f;
        isFling = false;
        onFlingStopped();
    }
//    float ppi = context.getResources().getDisplayMetrics().density * 160.0f;
//  see OverScroller.java
    float ppi = 2.625 * 160.0f;
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
    canvas->save();
    canvas->clipIRect(skRect);
    View::draw(canvas);
    for (auto child: children) {
        if (!ignoreChildDraw(child)) {
            child->draw(canvas);
        }
    }
    canvas->restore();
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

void ScrollView::scrollToIndex(int index, bool animated) {
    if (index < 0 && index > children.size() - 1) {
        ALOGE("ScrollView::scrollToChild error: %d", index)
        return;
    }
    auto translate = 0;
    if (_direction == YGFlexDirectionColumn) {
        for (int i = 0; i < index; ++i) {
            auto child = children[i];
            translate += child->getHeight() + child->marginTop + child->marginBottom;
        }
        if (animated) {
            scrollTo(-translate);
        } else {
            setTranslateY(-translate);
        }
    } else {
        for (int i = 0; i < index; ++i) {
            translate += children[i]->getWidth();
        }
        //Todo
    }
}

bool ScrollView::isScroller() {
    return true;
}

void ScrollView::scrollTo(float value) {
    auto start = _direction == YGFlexDirectionColumn ? translateY : translateX;
    auto scrollAnimator = new LinearAnimator(this, start, value);
    scrollAnimator->setDuration(500);
    scrollAnimator->setUpdateListener([this](View *view, float value) {
        if (_direction == YGFlexDirectionColumn) {
            setTranslateY(value);
        } else {
            //Todo
        }
    });
    scrollAnimator->start();
}

void ScrollView::scrollBy(float value) {
    auto start = _direction == YGFlexDirectionColumn ? translateY : translateX;
    scrollTo(start + value);
}

int ScrollView::getDistanceByIndex(int index) {
    if (index >= children.size()) {
        ALOGE("ScrollView::getChildHeightSum invalid param index:%d", index)
        return 0;
    }
    auto sum = 0;
    if (_direction == YGFlexDirectionColumn) {
        for (int i = 0; i < index; ++i) {
            auto child = children[index];
            sum += child->getHeight() + child->marginTop + child->marginBottom;
        }
    } else {
        for (int i = 0; i < index; ++i) {
            auto child = children[index];
            sum += child->getWidth() + child->marginLeft + child->marginRight;
        }
    }
    return sum;
}

void ScrollView::onFlingStopped() {

}
