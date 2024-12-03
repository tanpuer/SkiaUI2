#pragma once

#include "vector"
#include "stack"
#include "RecyclerViewHolder.h"
#include "unordered_map"

namespace HYSkiaUI {

template<typename T>
class RecyclerViewAdapter {

public:

    RecyclerViewAdapter(View *recyclerView) {

    }

    virtual ~RecyclerViewAdapter() {
        for (auto &vh: currVHList) {
            delete vh;
        }
        for (auto &cache: vhCache) {
            while (!cache.second->empty()) {
                auto vh = cache.second->top();
                cache.second->pop();
                delete vh;
            }
        }
    }

    virtual void setData(std::vector<T> data) {
        this->data = std::move(data);
    }

    virtual T getItem(int pos) {
        assert(pos >= 0 && pos < data.size());
        return data[pos];
    }

    virtual int getSize() {
        return data.size();
    }

    virtual int getItemType(int index) {
        return 0;
    }

#pragma mark ViewHolder

    virtual RecyclerViewHolder <T> *onCreateViewHolder(int viewType) = 0;

    virtual void onBindViewHolder(RecyclerViewHolder <T> *viewHolder, int index, T item) = 0;

    virtual void onRecycleViewHolder(RecyclerViewHolder <T> *viewHolder, T item) = 0;

    virtual void putViewHolderToCache(int itemType, RecyclerViewHolder <T> *vh) {
        if (vhCache.find(itemType) == vhCache.end()) {
            ALOGD("RecyclerView init cache %d", itemType)
            vhCache.emplace(itemType, new std::stack<RecyclerViewHolder<T> *>());
        }
        auto vhStack = vhCache[itemType];
        vhStack->push(vh);
        ALOGD("RecyclerView push to cache %d %d", itemType, vhStack->size())
    }

    virtual RecyclerViewHolder <T> *getViewHolderFromCache(int itemType) {
        if (vhCache.find(itemType) == vhCache.end()) {
            ALOGD("RecyclerView init cache %d", itemType)
            vhCache.emplace(itemType, new std::stack<RecyclerViewHolder<T> *>());
            return nullptr;
        }
        auto vhStack = vhCache[itemType];
        ALOGD("Recyclerview getViewHolderFromCache %d %d", itemType, vhStack->size())
        if (vhStack->empty()) {
            return nullptr;
        } else {
            auto vh = vhStack->top();
            vhStack->pop();
            return vh;
        }
    }

#pragma mark notify

    virtual void notifyDataSetChanged() {
        for (auto &item: currVHList) {
        }
    }

    virtual void notifyItemChanged(int index) {

    }

#pragma mark index

    /**
     * 列表中对应的data的区间
     */
    int startIndex = 0;
    int endIndex = 0;

    /**
     * 向下滑动时，回收头部的vh
     * @param vh
     */
    void recycleStartVH(RecyclerViewHolder <T> *vh) {
        ALOGD("RecyclerView recycleStartVH %d %d", startIndex, getItemType(startIndex))
        putViewHolderToCache(getItemType(startIndex), vh);
        onRecycleViewHolder(vh, data[startIndex]);
        startIndex++;
        currVHList.erase(currVHList.begin());
    }

    /**
     * 向上滑动时，回收尾部的vh
     * @param vh
     */
    void recyclerEndVH(RecyclerViewHolder <T> *vh) {
        ALOGD("RecyclerView recyclerEndVH %d", endIndex)
        putViewHolderToCache(getItemType(endIndex), vh);
        endIndex--;
        onRecycleViewHolder(vh, data[endIndex]);
        currVHList.erase(currVHList.end() - 1);
    }

    /**
     * 向上滑动时，处理头部的vh
     */
    RecyclerViewHolder <T> *handleStartVH() {
        startIndex--;
        auto itemType = getItemType(startIndex);
        RecyclerViewHolder<T> *vh = getViewHolderFromCache(itemType);
        if (vh == nullptr) {
            vh = onCreateViewHolder(itemType);
            ALOGD("RecyclerView create new VH for start %d, itemType: %d", startIndex, itemType)
        } else {
            ALOGD("RecyclerView getVHFromCache for start: %d, itemType: %d", startIndex, itemType)
        }
        auto item = data[startIndex];
        onBindViewHolder(vh, startIndex, item);
        currVHList.insert(currVHList.begin(), vh);
        return vh;
    }

    /**
     * 向下滑动时，处理尾部的vh
     * @return
     */
    RecyclerViewHolder <T> *handleEndVH() {
        auto itemType = getItemType(endIndex);
        auto vh = getViewHolderFromCache(itemType);
        if (vh == nullptr) {
            vh = onCreateViewHolder(itemType);
            ALOGD("RecyclerView create new VH for end %d, itemType: %d", endIndex, itemType)
        } else {
            ALOGD("RecyclerView get VH from Cache for end %d, itemType: %d", endIndex, itemType)
        }
        auto item = data[endIndex];
        onBindViewHolder(vh, endIndex, item);
        endIndex++;
        currVHList.insert(currVHList.end(), vh);
        return vh;
    }

    virtual void setOnItemClickListener(std::function<void(T, View *)> callback) {
        this->itemClickListener = itemClickListener;
    }

protected:

    std::vector<T> data;

    std::function<void(T, View *)> itemClickListener = nullptr;

private:

    /**
     * vh缓存，暂时用Map + Stack实现
     */
    std::unordered_map<int, std::stack<RecyclerViewHolder<T> *> *> vhCache;

public:
    /**
     * 当前RecyclerView所持有的vh集合
     */
    std::vector<RecyclerViewHolder<T> *> currVHList;

};

}
