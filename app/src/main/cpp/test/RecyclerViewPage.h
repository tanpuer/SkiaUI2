#pragma once

#include "Page.h"
#include "RecyclerViewHolder.h"
#include "RecyclerViewAdapter.h"
#include "RecyclerView.h"

namespace HYSkiaUI {

struct RecyclerData {
    int index;
};

class TestRecyclerView : public RecyclerView<RecyclerData> {

public:

    TestRecyclerView() {}

    ~TestRecyclerView() {}

    void draw(SkCanvas *canvas) override {
        ScrollView::draw(canvas);
        markDirty();
    }

};

class TestViewHolder : public RecyclerViewHolder<RecyclerData> {

public:

    TestViewHolder(View *itemView) : RecyclerViewHolder(itemView) {

    }

    void updateView(RecyclerData item) override;
};

class TestAdapter : public RecyclerViewAdapter<RecyclerData> {

public:

    TestAdapter(View *recyclerView, const std::shared_ptr<SkiaUIContext> &context)
            : RecyclerViewAdapter(recyclerView) {
        this->context = context;
        this->recyclerView = recyclerView;
    }

    RecyclerViewHolder<RecyclerData> *onCreateViewHolder(int viewType) override;

    void
    onBindViewHolder(RecyclerViewHolder<RecyclerData> *viewHolder, int index,
                     RecyclerData item) override;

    void
    onRecycleViewHolder(RecyclerViewHolder<RecyclerData> *viewHolder, RecyclerData item) override;

    View *getRecyclerView() {
        return recyclerView;
    }

private:
    std::shared_ptr<SkiaUIContext> context;

    View *recyclerView = nullptr;
};

class RecyclerViewPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initRecyclerView(ViewGroup *root, int width, int height);

};

}
