#pragma once

#include "ScrollView.h"
#include "RecyclerViewAdapter.h"
#include "RecyclerViewHolder.h"

namespace HYSkiaUI {

template<typename T>
class RecyclerView : public ScrollView {

public:

    RecyclerView() {

    }

    virtual ~RecyclerView() {
        delete adapter;
    }

    const char *name() override {
        return "RecyclerView";
    }

    bool addView(View *view) override {
        ALOGD("RecyclerView addView %ld", children.size())
        return ScrollView::addView(view);
    }

    bool addViewAt(View *view, uint32_t index) override {
        ALOGD("RecyclerView addView at %d %ld", index, children.size())
        return ScrollView::addViewAt(view, index);
    }

    bool removeViewAt(uint32_t index) override {
        ALOGD("RecyclerView removeView at %d, %ld", index, children.size())
        return ScrollView::removeViewAt(index);
    }

    virtual void measure() override {
        //简化处理RecyclerView必须指定宽高
        if (adapter == nullptr) {
            ALOGD("RecyclerView Adapter is null, ignore measure")
            ViewGroup::setMeasuredDimension(width, height);
            return;
        }
        assert(width > 0 && height > 0);
        if (_direction == YGFlexDirectionColumn) {
            int childHeightSum = 0;
            for (auto &child: children) {
                childHeightSum += child->getHeight();
            }

            if (!YGFloatsEqual(0.0f, translateY) && childHeightSum > height) {
                //从头清理
                for (auto itr = adapter->currVHList.cbegin(); itr != adapter->currVHList.cend();) {
                    if (*itr == nullptr) {
                        break;
                    }
                    auto itemView = (*itr)->getItemView();
                    if (ignoreChildDraw(itemView)) {
                        adapter->recycleStartVH(*itr);
                        itr = adapter->currVHList.cbegin();
                        translateY += children[0]->getRect().height();
                        removeViewAt(0);
                    } else {
                        break;
                    }
                }

                //从尾清理
                for (auto itr = adapter->currVHList.cend() - 1;
                     itr != adapter->currVHList.cbegin();) {
                    if (*itr == nullptr) {
                        break;
                    }
                    auto itemView = (*itr)->getItemView();
                    if (ignoreChildDraw(itemView)) {
                        adapter->recyclerEndVH(*itr);
                        itr = adapter->currVHList.cend() - 1;
                        removeViewAt(children.size() - 1);
                    } else {
                        break;
                    }
                }
            }

            //再setMeasureDimension调用前height为0，此时要用layoutParams->_height才行
            View *firstChild = nullptr;
            View *lastChild = nullptr;
            if (!children.empty()) {
                firstChild = children.front();
                lastChild = children.back();
            }
            auto addedHeight = 0;
            //todo
            while ((children.empty() && adapter->getSize() > 0) ||
                   (height == 0 && childHeightSum < height) ||
                   (firstChild != nullptr && height > 0 && !lastScrollDown &&
                    adapter->startIndex > 0 &&
                    firstChild->getRect().top() - addedHeight > skRect.top() - 50) ||
                   (lastChild != nullptr && height > 0 && lastScrollDown &&
                    adapter->endIndex < adapter->getSize() &&
                    lastChild->getRect().bottom() + addedHeight < skRect.bottom() + 50)) {
                RecyclerViewHolder<T> *vh = nullptr;
                if (lastScrollDown) {
                    vh = adapter->handleEndVH();
                } else {
                    vh = adapter->handleStartVH();
                }
                View *child = vh->getItemView();
                child->setWidth(getWidth());
                if (lastScrollDown) {
                    addView(child);
                } else {
                    addViewAt(child, 0);
                }
                child->measure();
                if (!lastScrollDown) {
                    translateY -= child->getHeight();
                }
                childHeightSum += child->getHeight();
                addedHeight += child->getHeight();
            }
            for (auto &child: children) {
                child->measure();
            }
        }
    }

    virtual bool canScroll() override {
        //todo 先写死无限滑动
        return true;
    }

    void updateTranslateY(float diffY) override {
        ScrollView::updateTranslateY(diffY);
    }

#pragma mark adapter

    virtual void setAdapter(RecyclerViewAdapter <T> *adapter) {
        this->adapter = adapter;
    }

    virtual RecyclerViewAdapter <T> *getAdapter() {
        return adapter;
    }

protected:

    RecyclerViewAdapter <T> *adapter = nullptr;

};

}
