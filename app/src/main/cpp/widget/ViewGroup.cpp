#include <yoga/Utils.h>
#include <base/native_log.h>
#include "ViewGroup.h"

ViewGroup::ViewGroup() : View(), maxChildWidth(0.0f), maxChildHeight(0.0f) {

}

ViewGroup::~ViewGroup() {
    if (node == nullptr) {
        return;
    }
    for (auto view: children) {
        delete view;
    }
}

bool ViewGroup::addView(View *view, LayoutParams *layoutParams) {
    return addViewAt(view, layoutParams, YGNodeGetChildCount(node));
}

bool ViewGroup::addViewAt(View *view, LayoutParams *layoutParams, uint32_t index) {
    if (view == nullptr || view->node == nullptr) {
        ALOGE("add null view, pls check view!")
        return false;
    }
    YGNodeInsertChild(node, view->node, index);
    view->parentName = name();
    view->parentId = viewId;
    children.insert(children.cbegin() + index, view);
    view->setLayoutParams(layoutParams);
    return true;
}

bool ViewGroup::removeView(View *view) {
    if (view == nullptr || view->node == nullptr) {
        ALOGE("remove null view, pls check view!")
        return false;
    }
    YGNodeRemoveChild(node, view->node);
    for (auto ite = children.begin(); ite < children.end(); ++ite) {
        if ((*ite)->viewId == view->viewId) {
            children.erase(ite);
            delete *ite;
            return true;
        }
    }
    return false;
}

bool ViewGroup::removeViewAt(uint32_t index) {
    if (index < 0 || index >= children.size()) {
        return false;
    }
    auto view = children[index];
    if (view == nullptr) {
        return false;
    }
    children.erase(children.cbegin() + index);
    //todo 是否要析构删除的view
    YGNodeRemoveChild(node, view->node);
    return true;
}

void ViewGroup::removeAllViews() {
    if (node == nullptr) {
        ALOGE("remove null view, pls check view!")
        return;
    }
    children.clear();
    YGNodeRemoveAllChildren(node);
}

void ViewGroup::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    width = _measuredWidth;
    height = _measuredHeight;
//    ALOGD("ViewGroup setMeasuredDimension %s %d %d", name(), _measuredWidth, _measuredHeight)
    YGNodeStyleSetWidth(node, static_cast<float>(_measuredWidth));
    YGNodeStyleSetHeight(node, static_cast<float>(_measuredHeight));
}

void ViewGroup::measureChild(View *child, int parentWidthMeasureSpec,
                             int parentHeightMeasureSpec) {
    auto lp = layoutParams.get();
    auto childWidthMeasureSpec = getChildMeasureSpec(child, parentWidthMeasureSpec,
                                                     lp->_paddingStart + lp->_paddingEnd,
                                                     child->layoutParams->_width);
    auto childHeightMeasureSpec = getChildMeasureSpec(child, parentHeightMeasureSpec,
                                                      lp->_paddingTop + lp->_paddingBottom,
                                                      child->layoutParams->_height);
//    ALOGD("measureChild %s %d %d", child->name(), MeasureSpec::getSize(childWidthMeasureSpec),
//          MeasureSpec::getSize(childHeightMeasureSpec));
    child->measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

int ViewGroup::getChildMeasureSpec(View *child, int parentMeasureSpec, int padding,
                                   int childDimension) {
    MeasureSpec::toString(parentMeasureSpec);
    auto specMode = MeasureSpec::getMode(parentMeasureSpec);
    auto specSize = MeasureSpec::getSize(parentMeasureSpec);
    auto size = std::max(0, specSize - padding);
    auto resultSize = 0;
    auto resultMode = UNSPECIFIED;
    switch (specMode) {
        // Parent has imposed an exact size on us
        case EXACTLY: {
//            ALOGD("getChildMeasureSpec exactly %s %d", child->name(), childDimension)
            if (childDimension >= 0) {
                resultSize = childDimension;
                resultMode = EXACTLY;
            } else if (MeasureSpec::isMatchParent(childDimension)) {
                // Child wants to be our size. So be it.
                resultSize = size;
                resultMode = EXACTLY;
            } else {
                // Child wants to determine its own size. It can't be
                // bigger than us.
                resultSize = size;
                resultMode = AT_MOST;
            }
            break;
        }
            // Parent has imposed a maximum size on us
        case AT_MOST: {
            if (childDimension >= 0) {
                // Child wants a specific size... so be it
                resultSize = childDimension;
                resultMode = EXACTLY;
            } else if (MeasureSpec::isMatchParent(childDimension)) {
                // Child wants to be our size, but our size is not fixed.
                // Constrain child to not be bigger than us.
                resultSize = size;
                resultMode = AT_MOST;
            } else {
                // Child wants to determine its own size. It can't be
                // bigger than us.
                resultSize = size;
                resultMode = AT_MOST;
            }
            break;
        }

            // Parent asked to see how big we want to be
        case UNSPECIFIED: {
            if (childDimension >= 0) {
                // Child wants a specific size... let him have it
                resultSize = childDimension;
                resultMode = EXACTLY;
            } else if (MeasureSpec::isMatchParent(childDimension)) {
                // Child wants to be our size... find out how big it should be
                resultSize = size;
                resultMode = UNSPECIFIED;
            } else {
                // Child wants to determine its own size.... find out how big it should be
                resultSize = size;
                resultMode = UNSPECIFIED;
            }
            break;
        }
        default: {
            ALOGD("illegal specMode")
            assert(nullptr);
        }
    }
    return MeasureSpec::makeMeasureSpec(resultSize, resultMode);
}

void ViewGroup::draw(SkCanvas *canvas) {
    for (auto child: children) {
        child->draw(canvas);
    }
    View::draw(canvas);
}

void ViewGroup::setAlignItems(YGAlign align) {
    SkASSERT(node);
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetAlignItems(node, align);
}

void ViewGroup::setJustifyContent(YGJustify justify) {
    SkASSERT(node);
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetJustifyContent(node, justify);
}

void ViewGroup::setAlignContent(YGAlign align) {
    SkASSERT(node);
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetAlignContent(node, align);
}

void ViewGroup::setFlexWrap(YGWrap wrap) {
    SkASSERT(node);
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetFlexWrap(node, wrap);
}

void ViewGroup::setFlexDirection(YGFlexDirection direction) {
    SkASSERT(node);
    if (node == nullptr) {
        return;
    }
    YGNodeStyleSetFlexDirection(node, direction);
}

int ViewGroup::getWidth() {
    if (YGNodeStyleGetWidth(node).unit == YGUnitAuto) {
        return getMaxWidthInChildren();
    } else {
        //exactly todo percent
        return View::getWidth();
    }
}

int ViewGroup::getHeight() {
    if (YGNodeStyleGetHeight(node).unit == YGUnitAuto) {
        return getMaxHeightInChildren();
    } else {
        //exactly todo percent
        return View::getHeight();
    }
}

int ViewGroup::getMaxHeightInChildren() {
    int maxHeight = 0;
    for (auto &child: children) {
        maxHeight = std::max(maxHeight,
                             child->getHeight() + child->marginTop + child->marginBottom);
    }
    return maxHeight;
}

int ViewGroup::getMaxWidthInChildren() {
    int maxWidth = 0;
    for (auto &child: children) {
        maxWidth = std::max(maxWidth, child->getWidth() + child->marginLeft + child->marginRight);
    }
    return maxWidth;
}

int ViewGroup::getChildHeightSum() {
    int sum = 0;
    for (auto &child: children) {
        SkASSERT(child != nullptr);
        sum += child->getHeight() + child->marginTop + child->marginBottom;
    }
    return sum;
}

int ViewGroup::getChildWidthSum() {
    int sum = 0;
    for (auto &child: children) {
        sum += child->getWidth() + child->marginLeft + child->marginRight;
    }
    return sum;
}

bool ViewGroup::isViewGroup() {
    return true;
}

const char *ViewGroup::name() {
    return "ViewGroup";
}

YGConfigRef ViewGroup::getConfig() {
    return config;
}

bool ViewGroup::dispatchTouchEvent(TouchEvent *touchEvent) {
    return touchEventDispatcher->dispatchTouchEvent(touchEvent);
}

bool ViewGroup::onInterceptTouchEvent(TouchEvent *touchEvent) {
    return touchEventDispatcher->onInterceptTouchEvent(touchEvent);
}

bool ViewGroup::onTouchEvent(TouchEvent *touchEvent) {
    return touchEventDispatcher->onTouchEvent(touchEvent);
}

void ViewGroup::requestDisallowInterceptTouchEvent(bool disallowIntercept) {
    touchEventDispatcher->requestDisallowInterceptTouchEvent(disallowIntercept);
}

bool ViewGroup::hasPercent() {
    if (View::hasPercent()) {
        return true;
    }
    for (auto &child: children) {
        if (child->hasPercent()) {
            return true;
        }
    }
    return false;
}

