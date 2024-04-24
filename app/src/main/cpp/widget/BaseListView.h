#pragma once

#include "BaseListAdapter.h"
#include "FlexboxLayout.h"

template<typename T>
class BaseListView : public FlexboxLayout {

public:
    enum class ScrollOrientation {
        TOP,
        BOTTOM,
    };

public:

    BaseListView() : FlexboxLayout() {
        this->adapter = nullptr;
        scrollOrientation = ScrollOrientation::BOTTOM;
    }

    virtual ~BaseListView() {
        if (adapter != nullptr) {
            delete adapter;
        }
    }

#pragma mark ListView api

    virtual void setAdapter(BaseListAdapter<T> *adapter) {
        this->adapter = adapter;
    }

    /**
     * 当ListView可见区域还没放满时，应当通过adapter创建子View
     * @return
     */
    virtual View *createView(int index) = 0;

    /**
     * 将view添加到FlexboxLayout中
     * @param view
     */
    virtual void attachChild(View *view) {
        view->setConfig(config);
        adapter->attachView(view);
    }

    /**
     * 从FlexboxLayout中移除该view
     * @param view
     */
    virtual void detachChild(View *view) {
        if (view == nullptr) {
            return;
        }
        FlexboxLayout::removeView(view);
        adapter->detachView(view);
    }

    virtual void setVelocity(float x, float y) {
        xVelocity = x;
        yVelocity = y;
    }

    virtual void updateTranslateY(float diffY) {
        translateY += diffY;
        //可滑动的上限和下限
//        auto maxTranslate = height - getChildHeightSum();
//        if (translateY <= maxTranslate) {
//            translateY = maxTranslate;
//        }
        if (translateY >= 0) {
            translateY = 0;
        }
        scrollOrientation = translateY > 0 ? ScrollOrientation::TOP : ScrollOrientation::BOTTOM;
    }

    virtual void updateTranslateX(float diffX) {
        translateX += diffX;
//        auto maxTranslate = width - getChildWidthSum();
//        if (translateX <= maxTranslate) {
//            translateX = maxTranslate;
//        }
        if (translateX >= 0) {
            translateX = 0;
        }
    }

    virtual void startFling() {

    }

    virtual void stopFling() {

    }

    virtual bool canScroll() {
        if (children.size() < adapter->getItemCount()) {
            return true;
        }
        if (_direction == YGFlexDirectionRow) {
            return abs(translateX) < getChildWidthSum() - width;
        } else {
            //todo 需要优化
            return abs(translateY) < getChildHeightSum() - height;
        }
    }

#pragma mark View api

    /**
     * ListView的宽高要固定
     * 在onMeasure过程中发现还没铺满，要创建子view并添加进来
     */
    virtual void measure() override {
        //ListView和ScrollView不同，ScrollView可以一口气算出总高度，ListView此处采取预绘制，在现有高度基础上
        //目前如果bottom < 0 || top > height就要remove
        //remove的时候还要更新translate: translateY += (被移除view的高度 + marginTop + marginBottom)
        SkASSERT(adapter != nullptr);
        ViewGroup::setMeasuredDimension(width, height);

        //todo 现在假设ListView铺满可滚动
        while (height - translateY > getChildHeightSum() && adapter->canCreateView()) {
            if (scrollOrientation == ScrollOrientation::BOTTOM) {
                //往下滑
                auto index = adapter->getCurrentIndex();
                auto child = adapter->createView(index);
                //加入view的时候要attach，remove的时候要detach
                attachChild(child);
                adapter->bindView(child, adapter->getItem(index));
                measureChild(child);

                //remove top
                while (children.size() >= 2 && children[0]->skRect.bottom() < -100) {
                    auto removedIndex = 0;
                    auto removedChild = children[removedIndex];
                    detachChild(removedChild);
                    translateY += (removedChild->getHeight() + removedChild->marginTop +
                                   removedChild->marginBottom);
                }
            } else if (scrollOrientation == ScrollOrientation::TOP) {
                //往上滑s
                auto index = adapter->getStartIndex();
                auto child = adapter->createView(index);
                //加在头部

                //remove bottom
                while (children.size() >= 2 &&
                       children[(children.size() - 1)]->skRect.bottom() > height + 100) {
                    auto removedIndex = children.size() - 1;
                    auto removedChild = children[index];
                    detachChild(removedChild);
                }
            } else {
                SK_ABORT("scrollOrientation is illegal!!!");
            }
        }
    }

    virtual void layout(int l, int t, int r, int b) override {
        View::layout(l, t, r, b);
//        if (isFling) {
//            calculateFlingTranslate();
//        }
        if (_direction == YGFlexDirectionRow) {
            layoutHorizontal(l + translateX, t, r, b);
        } else {
            layoutVertical(l, t + translateY, r, b);
        }
    }

protected:

    BaseListAdapter<T> *adapter;

    float xVelocity, yVelocity;

    ScrollOrientation scrollOrientation;

};

//todo 要不要不用模版处理？
template<typename T>
class ListViewDispatcher : public TouchEventDispatcher {

public:

    ListViewDispatcher(BaseListView<T> *view) : TouchEventDispatcher(view) {
        this->listView = view;
    }

    virtual ~ListViewDispatcher() {}

    virtual bool onTouchEvent(TouchEvent *touchEvent) override {
        switch (touchEvent->action) {
            case TouchEvent::ACTION_DOWN: {
                listView->stopFling();
                if (listView->_direction == YGFlexDirectionColumn) {
                    startY = touchEvent->y;
                } else {
                    startX = touchEvent->x;
                }
                break;
            }
            case TouchEvent::ACTION_MOVE: {
                if (listView->_direction == YGFlexDirectionColumn) {
                    listView->updateTranslateY(touchEvent->y - startY);
                    startY = touchEvent->y;
                } else {
                    listView->updateTranslateX(touchEvent->x - startX);
                    startX = touchEvent->x;
                }
                break;
            }
            case TouchEvent::ACTION_UP: {
                listView->startFling();
                break;
            }
            case TouchEvent::ACTION_CANCEL: {
                break;
            }
            default: {
                break;
            }
        }
        return true;
    }

    virtual void findTargetView(TouchEvent *touchEvent) override {
        TouchEventDispatcher::findTargetView(touchEvent);
    }

    virtual void dispatchToTargetView(TouchEvent *touchEvent) override {
        TouchEventDispatcher::dispatchToTargetView(touchEvent);
        if (canScroll()) {
            listView->onTouchEvent(touchEvent);
        }
    }

    virtual View *
    findTargetViewTraversal(ViewGroup *viewGroup, TouchEvent *touchEvent, float tempLeft,
                            float tempTop) override {
        //和不可滑动的页面相比多个偏移计算
        auto diffX = 0.0f;
        auto diffY = 0.0f;
        if (listView != nullptr) {
            if (listView->_direction == YGFlexDirectionRow) {
                diffX = listView->translateX;
            } else {
                diffY = listView->translateY;
            }
        }
        for (auto i = viewGroup->children.rbegin(); i != viewGroup->children.rend(); ++i) {
            auto child = *i;
            auto left = YGNodeLayoutGetLeft(child->node) + tempLeft + diffX;
            auto top = YGNodeLayoutGetTop(child->node) + tempTop + diffY;
            auto width = YGNodeLayoutGetWidth(child->node);
            auto height = YGNodeLayoutGetHeight(child->node);
            ALOGD("findTargetViewTraversal %f %f %f %f %f %f", left, top, width, height,
                  touchEvent->x,
                  touchEvent->y)
            if (touchEvent->x >= left && touchEvent->x <= left + width &&
                touchEvent->y >= top && touchEvent->y <= top + height) {
                if (child->isViewGroup()) {
                    return findTargetViewTraversal(dynamic_cast<ViewGroup *>(child), touchEvent,
                                                   left, top);
                } else {
                    return child;
                }
            }
        }
        ALOGD("findTargetViewTraversal null")
        return viewGroup;
    }

    virtual bool canScroll() {
        if (listView == nullptr) {
            return false;
        }
        return listView->canScroll();
    }

protected:

    BaseListView<T> *listView;

    float startX = 0.0f;
    float startY = 0.0f;

};