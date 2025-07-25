#include <base/native_log.h>
#include "ViewGroup.h"

namespace HYSkiaUI {

ViewGroup::ViewGroup() : View() {

}

ViewGroup::~ViewGroup() {
    for (auto view: children) {
        delete view;
    }
}

bool ViewGroup::addView(View *view) {
    return addViewAt(view, YGNodeGetChildCount(node));
}

bool ViewGroup::addViewAt(View *view, uint32_t index) {
    if (view == nullptr || view->getNode() == nullptr) {
        ALOGE("add null view, pls check view!")
        return false;
    }
    YGNodeInsertChild(node, view->getNode(), index);
    view->setParent(this);
    children.insert(children.cbegin() + index, view);
    return true;
}

bool ViewGroup::removeView(View *view) {
    if (view == nullptr || view->getNode() == nullptr) {
        ALOGE("remove null view, pls check view!")
        return false;
    }
    YGNodeRemoveChild(node, view->getNode());
    auto it = std::find(children.begin(), children.end(), view);
    if (it != children.end()) {
        children.erase(it);
    }
    delete view;
    return false;
}

bool ViewGroup::removeViewAt(uint32_t index) {
    if (index >= children.size()) {
        return false;
    }
    auto view = children[index];
    if (view == nullptr) {
        return false;
    }
    return removeView(view);
}

void ViewGroup::setMeasuredDimension(int _measuredWidth, int _measuredHeight) {
    width = _measuredWidth;
    height = _measuredHeight;
    YGNodeStyleSetWidth(node, static_cast<float>(_measuredWidth));
    YGNodeStyleSetHeight(node, static_cast<float>(_measuredHeight));
}

void ViewGroup::measureChild(View *child) {
    child->measure();
}

void ViewGroup::draw(SkCanvas *canvas) {
    View::draw(canvas);
    for (auto child: children) {
        child->draw(canvas);
    }
}

void ViewGroup::setAlignItems(YGAlign align) {
    SkASSERT(node);
    YGNodeStyleSetAlignItems(node, align);
}

void ViewGroup::setJustifyContent(YGJustify justify) {
    SkASSERT(node);
    YGNodeStyleSetJustifyContent(node, justify);
}

void ViewGroup::setAlignContent(YGAlign align) {
    SkASSERT(node);
    YGNodeStyleSetAlignContent(node, align);
}

void ViewGroup::setFlexWrap(YGWrap wrap) {
    SkASSERT(node);
    YGNodeStyleSetFlexWrap(node, wrap);
}

void ViewGroup::setFlexDirection(YGFlexDirection direction) {
    SkASSERT(node);
    YGNodeStyleSetFlexDirection(node, direction);
}

const char *ViewGroup::getFlexDirection() {
    SkASSERT(node);
    auto direction = YGNodeStyleGetFlexDirection(node);
    return YGFlexDirectionToString(direction);
}

int ViewGroup::getChildHeightSum() {
    int sum = 0;
    for (auto &child: children) {
        SkASSERT(child != nullptr);
        sum += child->getHeight() + child->getMarginTop() + child->getMarginBottom();
    }
    return sum;
}

int ViewGroup::getChildWidthSum() {
    int sum = 0;
    for (auto &child: children) {
        sum += child->getWidth() + child->getMarginLeft() + child->getMarginRight();
    }
    return sum;
}

bool ViewGroup::isViewGroup() {
    return true;
}

const char *ViewGroup::name() {
    return "ViewGroup";
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

bool ViewGroup::dispatchVelocity(Velocity *velocity) {
    return touchEventDispatcher->dispatchVelocity(velocity);
}

void ViewGroup::layout(int l, int t, int r, int b) {
}

const char *ViewGroup::getFLexWrap() {
    auto flexWrap = YGNodeStyleGetFlexWrap(node);
    return YGWrapToString(flexWrap);
}

const char *ViewGroup::getJustifyContent() {
    auto justifyContent = YGNodeStyleGetJustifyContent(node);
    return YGJustifyToString(justifyContent);
}

const char *ViewGroup::getAlignItems() {
    auto alignItems = YGNodeStyleGetAlignItems(node);
    return YGAlignToString(alignItems);
}

void ViewGroup::onShow() {
    View::onShow();
    for (const auto &item: children) {
        item->onShow();
    }
}

void ViewGroup::onHide() {
    View::onHide();
    for (const auto &item: children) {
        item->onHide();
    }
}

void ViewGroup::performAnimations() {
    View::performAnimations();
    for (const auto &item: children) {
        item->performAnimations();
    }
}

bool ViewGroup::addViewBefore(View *view, View *beforeView) {
    size_t index;
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i] == beforeView) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return false;
    }
    return addViewAt(view, static_cast<uint32_t>(index));
}

void ViewGroup::removeViews(uint32_t index, uint32_t count) {
    for (auto i = index; i < index + count; ++i) {
        auto view = children[i];
        YGNodeRemoveChild(node, view->getNode());
        delete view;
    }
    children.erase(children.begin() + index, children.begin() + index + count);
}

void ViewGroup::moveViews(uint32_t from, uint32_t to, uint32_t count) {
    std::vector<View *> movedViews;
    for (auto i = from; i < from + count; ++i) {
        auto view = children[i];
        movedViews.push_back(view);
        YGNodeRemoveChild(node, view->getNode());
    }
    children.erase(children.begin() + from, children.begin() + from + count);
    for (int i = 0; i < movedViews.size(); ++i) {
        children.insert(children.cbegin() + to - count + i, movedViews[i]);
        YGNodeInsertChild(node, movedViews[i]->getNode(), to - count + i);
    }
}

View *ViewGroup::removeViewAtForRV(uint32_t index) {
    auto view = children[index];
    if (view == nullptr) {
        return nullptr;
    }
    YGNodeRemoveChild(node, view->getNode());
    auto it = std::find(children.begin(), children.end(), view);
    if (it != children.end()) {
        children.erase(it);
    }
    return view;
}

}
