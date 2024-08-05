#pragma once

#include "RecyclerView.h"
#include "RecyclerViewHolder.h"

struct LyricLRC {
    long timeMills;
    std::string content;
};

class LyricViewHolder : public RecyclerViewHolder<LyricLRC> {

public:

    LyricViewHolder(View *itemView) : RecyclerViewHolder(itemView) {

    }

    void updateView(LyricLRC item) override;
};

class LyricAdapter : public RecyclerViewAdapter<LyricLRC> {

public:

    LyricAdapter(const std::shared_ptr<SkiaUIContext> &context, View *recyclerView)
            : RecyclerViewAdapter(recyclerView) {
        this->context = context;
        this->recyclerView = recyclerView;
    }

    RecyclerViewHolder<LyricLRC> *onCreateViewHolder(int viewType) override;

    void
    onBindViewHolder(RecyclerViewHolder<LyricLRC> *viewHolder, int index, LyricLRC item) override;

    void onRecycleViewHolder(RecyclerViewHolder<LyricLRC> *viewHolder, LyricLRC item) override;

    View *getRecyclerView() {
        return recyclerView;
    }

private:
    std::shared_ptr<SkiaUIContext> context;

    View *recyclerView = nullptr;
};

class LyricView : public RecyclerView<LyricLRC> {

public:

    LyricView();

    ~LyricView();

    void setSource(const char *source);

};
