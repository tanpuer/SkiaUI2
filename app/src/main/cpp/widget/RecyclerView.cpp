#include "RecyclerView.h"

namespace HYSkiaUI {

RecyclerView::RecyclerView() {

}

RecyclerView::~RecyclerView() {
    for (const auto &item: viewCache) {
        delete item;
    }
}

void RecyclerView::measure() {
    ScrollView::measure();
    if (firstFlag) {
        firstFlag = false;
        initChildren();
    }
}

void RecyclerView::draw(SkCanvas *canvas) {
    ScrollView::draw(canvas);
}

void RecyclerView::setDataSize(uint32_t size) {
    this->size = size;
    markDirty();
}

void RecyclerView::initChildren() {
    auto childHeightSum = 0;
    while (childHeightSum < height) {
        auto child = onCreateView(children.size() + firstChildIndex);
        onBindView(children.size() + firstChildIndex, child);
        child->measure();
        childHeightSum += child->getHeight() + child->getMarginTop() + child->getMarginBottom();
        ScrollView::addView(child);
    }
}

void RecyclerView::layout(int l, int t, int r, int b) {
    ScrollView::layout(l, t, r, b);
    if (lastScrollEnd) {
        //scrollToBottom, addView to bottom, then removeView from top
        auto lastChild = children.back();
        if (lastChild == nullptr) {
            return;
        }
        auto lastChildRect = lastChild->getRect();
        auto lastChildBottom = lastChildRect.bottom();
        auto targetBottom = getRect().bottom() + DISTANCE;
        while (children.size() < size && lastChildBottom < targetBottom) {
            View *child = nullptr;
            if (viewCache.empty()) {
                ALOGD("RecyclerView createView")
                child = onCreateView(children.size() + firstChildIndex);
            } else {
                ALOGD("RecyclerView addView from cache")
                child = viewCache.back();
                viewCache.pop_back();
            }
            onBindView(children.size() + firstChildIndex, child);
            child->measure();
            auto diffY = child->getHeight() + child->getMarginTop() + child->getMarginBottom();
            lastChildBottom += diffY;
            ScrollView::addView(child);
        }
        auto firstChild = children.front();
        if (firstChild == nullptr) {
            return;
        }
        auto topChildRect = firstChild->getRect();
        auto topChildTop = topChildRect.top();
        auto targetTop = getRect().top() - DISTANCE;
        while (topChildTop < targetTop) {
            ALOGD("RecyclerView remove first view")
            auto removedView = ScrollView::removeViewAtForRV(0);
            auto diffY = removedView->getHeight() + removedView->getMarginTop() +
                         removedView->getMarginBottom();
            topChildTop += diffY;
            firstChildIndex++;
            updateTranslateY(diffY);
            viewCache.emplace_back(removedView);
        }
        ALOGD("RecyclerView size: %ld firstChildIndex:%d", children.size(), firstChildIndex)
    } else {
        //scrollToTop, addView to top, then removeView from bottom
        auto firstChild = children.front();
        if (firstChild == nullptr) {
            return;
        }
        auto firstChildRect = firstChild->getRect();
        auto firstChildTop = firstChildRect.top();
        auto targetTop = getRect().top() - DISTANCE;
        while (firstChildIndex > 0 && firstChildTop > targetTop) {
            View *child = nullptr;
            if (viewCache.empty()) {
                ALOGD("RecyclerView createView")
                child = onCreateView(firstChildIndex - 1);
            } else {
                ALOGD("RecyclerView addView from cache")
                child = viewCache.back();
                viewCache.pop_back();
            }
            onBindView(firstChildIndex - 1, child);
            child->measure();
            auto diffY = child->getHeight() + child->getMarginTop() + child->getMarginBottom();
            firstChildTop -= diffY;
            ScrollView::addViewAt(child, 0);
            firstChildIndex--;
            updateTranslateY(-diffY);
            if (firstChildIndex == 0) {
                break;
            }
        }
        auto lastChild = children.back();
        if (lastChild == nullptr) {
            return;
        }
        auto lastChildRect = lastChild->getRect();
        auto lastChildTop = lastChildRect.top();
        auto targetBottom = getRect().bottom() + DISTANCE;
        while (children.size() < size && lastChildTop > targetBottom) {
            ALOGD("RecyclerView remove back view size:%ld firstChildIndex:%d", children.size(),
                  firstChildIndex)
            auto removedView = ScrollView::removeViewAtForRV(children.size() - 1);
            auto diffY = removedView->getHeight() + removedView->getMarginTop() +
                         removedView->getMarginBottom();
            lastChildTop -= diffY;
            viewCache.emplace_back(removedView);
        }
    }
}

}
