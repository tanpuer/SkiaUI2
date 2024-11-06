#include <yoga/Utils.h>
#include <base/native_log.h>
#include "ViewGroup.h"

ViewGroup::ViewGroup() : View() {

}

ViewGroup::~ViewGroup() {
    if (node == nullptr) {
        return;
    }
    for (auto view: children) {
        delete view;
    }
}

bool ViewGroup::addView(View *view) {
    return addViewAt(view, YGNodeGetChildCount(node));
}

bool ViewGroup::addViewAt(View *view, uint32_t index) {
    if (view == nullptr || view->node == nullptr) {
        ALOGE("add null view, pls check view!")
        return false;
    }
    YGNodeInsertChild(node, view->node, index);
    view->parentName = name();
    view->parentId = viewId;
    view->parent = this;
    children.insert(children.cbegin() + index, view);
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
            (*ite)->markForDelete = true;
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
    YGNodeRemoveChild(node, view->node);
    view->markForDelete = true;
    return true;
}

void ViewGroup::removeAllViews() {
    if (node == nullptr) {
        ALOGE("remove null view, pls check view!")
        return;
    }
    for (const auto &item: children) {
        item->markForDelete = true;
    }
    YGNodeRemoveAllChildren(node);
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
    children.erase(std::remove_if(children.begin(),
                                  children.end(),
                                  [](View *child) {
                                      if (child->markForDelete) {
                                          delete child;
                                          return true;
                                      } else {
                                          return false;
                                      }
                                  }), children.end());
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

const char *ViewGroup::getFlexDirection() {
    SkASSERT(node);
    auto direction = YGNodeStyleGetFlexDirection(node);
    return YGFlexDirectionToString(direction);
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

bool ViewGroup::dispatchVelocity(Velocity *velocity) {
    return touchEventDispatcher->dispatchVelocity(velocity);
}

void ViewGroup::layout(int l, int t, int r, int b) {
    assert(false);
}

const char *ViewGroup::getFLexWrap() {
    assert(node);
    auto flexWrap = YGNodeStyleGetFlexWrap(node);
    return YGWrapToString(flexWrap);
}

const char *ViewGroup::getJustifyContent() {
    assert(node);
    auto justifyContent = YGNodeStyleGetJustifyContent(node);
    return YGJustifyToString(justifyContent);
}

const char *ViewGroup::getAlignItems() {
    assert(node);
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

