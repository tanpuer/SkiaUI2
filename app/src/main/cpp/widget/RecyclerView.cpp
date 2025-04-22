#include "RecyclerView.h"

namespace HYSkiaUI {

RecyclerView::RecyclerView() {

}

RecyclerView::~RecyclerView() {
    for (const auto &item: viewCache) {
        for (const auto &view: item.second) {
            delete view;
        }
    }
}

void RecyclerView::measure() {
    ScrollView::measure();
    if (firstFlag) {
        firstFlag = false;
        initChildren();
    }
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
        while (firstChildIndex + children.size() < size && lastChildBottom < targetBottom) {
            auto targetIndex = children.size() + firstChildIndex;
            auto child = getViewFromCache(targetIndex);
            if (child == nullptr) {
                ALOGD("RecyclerView createView")
                child = onCreateView(targetIndex);
            }
            onBindView(targetIndex, child);
            child->measure();
            auto diffY = child->getHeight() + child->getMarginTop() + child->getMarginBottom();
            lastChildBottom += diffY;
            ScrollView::addView(child);
            YGNodeCalculateLayout(node, width, height, YGDirection::YGDirectionLTR);
            layoutNewAddedChild(l, t, r, b, child);
            ALOGD("RecyclerView size: %ld firstChildIndex:%d", children.size(), firstChildIndex)
        }
        auto firstChild = children.front();
        if (firstChild == nullptr) {
            return;
        }
        auto topChildRect = firstChild->getRect();
        auto topChildBottom = topChildRect.bottom();
        auto targetTop = getRect().top() - DISTANCE;
        while (topChildBottom < targetTop) {
            auto removedIndex = firstChildIndex;
            auto removedView = ScrollView::removeViewAtForRV(0);
            ALOGD("RecyclerView remove first view index:%d name:%s", removedIndex,
                  removedView->name())
            firstChildIndex++;
            auto diffY = removedView->getHeight() + removedView->getMarginTop() +
                         removedView->getMarginBottom();
            topChildBottom += diffY;
            putViewToCache(removedIndex, removedView);
            updateTranslateY(diffY);
            ALOGD("RecyclerView size: %ld firstChildIndex:%d", children.size(), firstChildIndex)
        }
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
            auto targetIndex = firstChildIndex - 1;
            View *child = getViewFromCache(targetIndex);
            if (child == nullptr) {
                ALOGD("RecyclerView createView")
                child = onCreateView(targetIndex);
            }
            onBindView(targetIndex, child);
            child->measure();
            auto diffY = child->getHeight() + child->getMarginTop() + child->getMarginBottom();
            firstChildTop -= diffY;
            ScrollView::addViewAt(child, 0);
            YGNodeCalculateLayout(node, width, height, YGDirection::YGDirectionLTR);
            layoutNewAddedChild(l, t, r, b, child);
            firstChildIndex--;
            updateTranslateY(-diffY);
            ALOGD("RecyclerView size: %ld firstChildIndex:%d", children.size(), firstChildIndex)
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
            auto removedIndex = firstChildIndex + children.size() - 1;
            auto removedView = ScrollView::removeViewAtForRV(children.size() - 1);
            putViewToCache(removedIndex, removedView);
            auto diffY = removedView->getHeight() + removedView->getMarginTop() +
                         removedView->getMarginBottom();
            lastChildTop -= diffY;
            ALOGD("RecyclerView size: %ld firstChildIndex:%d", children.size(), firstChildIndex)
        }
    }
}

View *RecyclerView::getViewFromCache(int index) {
    auto type = getViewType(index);
    if (viewCache.find(type) == viewCache.end()) {
        viewCache.emplace(std::make_pair(type, std::vector<View *>()));
    }
    auto &typeCache = viewCache[type];
    if (typeCache.empty()) {
        return nullptr;
    }
    auto child = typeCache.back();
    typeCache.pop_back();
    ALOGD("RecyclerView addView from cache index:%d type:%d", index, type)
    return child;
}

void RecyclerView::putViewToCache(int index, View *view) {
    auto type = getViewType(index);
    if (viewCache.find(type) == viewCache.end()) {
        viewCache.emplace(std::make_pair(type, std::vector<View *>()));
    }
    auto &typeCache = viewCache[type]; //auto typeCache = viewCache[type] error!
    typeCache.push_back(view);
}

void RecyclerView::layoutNewAddedChild(int l, int t, int r, int b, View *view) {
    auto childNode = view->getNode();
    auto left = static_cast<int>(YGNodeLayoutGetLeft(childNode));
    auto top = static_cast<int>(YGNodeLayoutGetTop(childNode));
    auto width = static_cast<int>(YGNodeLayoutGetWidth(childNode));
    auto height = static_cast<int>(YGNodeLayoutGetHeight(childNode));
    view->layout(left + l, top + t + translateY, left + l + width,
                 top + t + translateY + height);
}

const char *RecyclerView::name() {
    return "RecyclerView";
}

}
