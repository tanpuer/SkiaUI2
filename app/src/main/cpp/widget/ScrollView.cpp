#include "ScrollDispatcher.h"
#include "ScrollView.h"
#include "algorithm"
#include "LinearAnimator.h"
#include "w3c_util.h"

namespace HYSkiaUI {

ScrollView::ScrollView() {
    touchEventDispatcher = std::make_unique<ScrollDispatcher>(this);
    mScroller = std::make_unique<OverScroller>();
}

ScrollView::~ScrollView() {
}

void ScrollView::measure() {
    for (auto &child: children) {
        measureChild(child);
    }
}

void ScrollView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (mScroller->computeScrollOffset()) {
        if (_direction == YGFlexDirectionRow) {
            setTranslateX(mScroller->getCurrentX());
        } else {
            setTranslateY(mScroller->getCurrentY());
        }
    }
    if (_direction == YGFlexDirectionRow) {
        layoutChildren(l + translateX, t, r + translateX, b);
        childrenWidthSum = getChildWidthSum();
    } else {
        layoutChildren(l, t + clampY, r, b + clampY);
        childrenHeightSum = getChildHeightSum();
    }
}

void ScrollView::updateTranslateY(float diffY) {
    translateY += diffY;
    setTranslateY(translateY);
}

void ScrollView::setTranslateY(float y) {
    markDirty();
    translateY = y;
    auto minY = std::min(0, height - childrenHeightSum);
    if (translateY > 0.0f) {
        clampY = applyDamping(translateY, height / 3);
    } else if (translateY < minY) {
        clampY = minY + applyDamping(translateY - minY, height / 3);
    } else {
        clampY = translateY;
    }
}

void ScrollView::updateTranslateX(float diffX) {
    translateX += diffX;
    setTranslateX(translateX);
}

void ScrollView::setTranslateX(float x) {
    markDirty();
    translateX = x;
    auto minX = std::min(0, width - childrenWidthSum);
    if (translateX > 0.0f) {
        clampX = applyDamping(translateX, width / 3);
    } else if (translateX < minX) {
        clampX = minX + applyDamping(translateX - minX, width / 3);
    } else {
        clampX = translateX;
    }
}

void ScrollView::setFlexWrap(YGWrap wrap) {
    assert(wrap == YGWrapNoWrap);
    FlexboxLayout::setFlexWrap(wrap);
}

void ScrollView::setFlexDirection(YGFlexDirection direction) {
    FlexboxLayout::setFlexDirection(direction);
}

void ScrollView::fling() {
    if (_direction == YGFlexDirectionColumn) {
        adjustVelocity(yVelocity);
        float minY = std::min(0, height - childrenHeightSum);
        if (translateY > 0.0f) {
            mScroller->springBack(0.0f, translateY, 0.0f, 0.0f, 0.0f, 0.0f);
        } else if (translateY < minY) {
            mScroller->springBack(0.0f, translateY, 0.0f, 0.0f, minY, minY);
        } else {
            mScroller->fling(0.0f, translateY, 0.0f, yVelocity, 0.0f, minY, 0.0f, 0.0f, 0.0f,
                             height / 3);
        }
    } else {
        adjustVelocity(xVelocity);
        float maxX = std::min(0, width - childrenWidthSum);
        if (translateX > 0.0f) {
            mScroller->springBack(translateX, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        } else if (translateX < maxX) {
            mScroller->springBack(translateX, 0.0f, 0.0f, maxX, 0.0f, 0.0f);
        } else {
            mScroller->fling(translateX, 0.0f, xVelocity, 0.0f, 0.0f, 0.0f, maxX, 0.0f, width / 3,
                             0.0f);
        }
    }
}

void ScrollView::stopFling() {
    if (!mScroller->isFinished()) {
        translateX = mScroller->getCurrentX();
        translateY = mScroller->getCurrentY();
        mScroller->abortAnimation();
    }
}

void ScrollView::draw(SkCanvas *canvas) {
    canvas->save();
    canvas->clipRect(skRect);
    View::draw(canvas);
    for (auto child: children) {
        if (!ignoreChildDraw(child)) {
            child->draw(canvas);
        }
    }
    canvas->restore();
}

bool ScrollView::ignoreChildDraw(View *child) {
    if (_direction == YGFlexDirectionColumn) {
        const auto childRect = child->getRect();
        auto childTop = childRect.top();
        auto childBottom = childRect.bottom();
        auto scrollTop = skRect.top();
        auto scrollBottom = skRect.bottom();
        //< top-100 || > bottom+100，invisible, ignore draw
        return childTop > scrollBottom + 100 || childBottom < scrollTop - 100;
    } else {
        const auto childRect = child->getRect();
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
            translate += child->getHeight() + child->getMarginTop() + child->getMarginBottom();
        }
        if (animated) {
            scrollTo(-translate);
        } else {
            setTranslateY(-translate);
        }
    } else {
        for (int i = 0; i < index; ++i) {
            auto child = children[i];
            translate += children[i]->getWidth() + child->getMarginLeft() + child->getMarginRight();
        }
        if (animated) {
            scrollTo(-translate);
        } else {
            setTranslateX(-translate);
        }
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
            if (!YGFloatsEqual(this->translateY, value)) {
                setTranslateY(value);
            }
        } else {
            if (!YGFloatsEqual(this->translateX, value)) {
                setTranslateX(value);
            }
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
        ALOGE("ScrollView::getDistanceByIndex invalid param index:%d", index)
        return 0;
    }
    auto sum = 0;
    if (_direction == YGFlexDirectionColumn) {
        for (int i = 0; i < index; ++i) {
            auto child = children[index];
            sum += child->getHeight() + child->getMarginTop() + child->getMarginBottom();
        }
    } else {
        for (int i = 0; i < index; ++i) {
            auto child = children[index];
            sum += child->getWidth() + child->getMarginLeft() + child->getMarginRight();
        }
    }
    return sum;
}

void ScrollView::setScrollEnd(bool flag) {
    this->lastScrollEnd = flag;
}

void ScrollView::adjustVelocity(float &velocity) {
    if (abs(velocity) <= MIN_VELOCITY) {
        velocity = .0f;
    } else if (abs(velocity) >= MAX_VELOCITY) {
        velocity = velocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
    }
}

float ScrollView::applyDamping(float offset, float maxDistance) {
    float absOffset = std::abs(offset);
    float dampedOffset = maxDistance * (1.0f - 1.0f / (1.0f + absOffset / maxDistance));
    return (offset > 0) ? dampedOffset : -dampedOffset;
}

int ScrollView::getChildWidthSum() {
    if (children.empty()) {
        return 0;
    }
    auto firstChild = children[0];
    auto lastChild = children[children.size() - 1];
    return lastChild->getRight() - firstChild->getLeft() + lastChild->getMarginRight() +
           firstChild->getMarginLeft();
}

int ScrollView::getChildHeightSum() {
    if (children.empty()) {
        return 0;
    }
    auto firstChild = children[0];
    auto lastChild = children[children.size() - 1];
    return lastChild->getBottom() - firstChild->getTop() + lastChild->getMarginBottom() +
           firstChild->getMarginTop();
}

}
