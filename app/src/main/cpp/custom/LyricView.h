#pragma once

#include "RecyclerView.h"
#include "RecyclerViewHolder.h"

namespace HYSkiaUI {

enum class LyricType {
    Lrc,
    Srt,
};

struct Lyric {
    std::string content;
    uint32_t type; //0=lrc; 1=srt
    std::vector<long> timeMills;
    std::vector<std::string> contentList; //逐字专用
    uint32_t lineIndex;
};

class LyricViewHolder : public RecyclerViewHolder<Lyric> {

public:

    LyricViewHolder(View *itemView) : RecyclerViewHolder(itemView) {

    }

    void updateView(Lyric item) override;
};

class LyricAdapter : public RecyclerViewAdapter<Lyric> {

public:

    LyricAdapter(const std::shared_ptr<SkiaUIContext> &context, View *recyclerView)
            : RecyclerViewAdapter(recyclerView) {
        this->context = context;
        this->recyclerView = recyclerView;
    }

    RecyclerViewHolder<Lyric> *onCreateViewHolder(int viewType) override;

    void
    onBindViewHolder(RecyclerViewHolder<Lyric> *viewHolder, int index, Lyric item) override;

    void onRecycleViewHolder(RecyclerViewHolder<Lyric> *viewHolder, Lyric item) override;

    View *getRecyclerView() {
        return recyclerView;
    }

private:
    std::shared_ptr<SkiaUIContext> context;

    View *recyclerView = nullptr;
};

class LyricView : public RecyclerView<Lyric> {

public:

    LyricView();

    ~LyricView();

    void setSourceLRC(const char *source);

    void setSourceSRT(const char *source);

    void draw(SkCanvas *canvas) override;

    void drawLyricLRC();

    void drawLyricSRT();

    void setCurrPositionFunc(std::function<long()> &&func);

private:

    long startTimeMills = 0L;

    LyricType type = LyricType::Lrc;

    std::function<long()> currentPositionFunc = nullptr;

    int currentIndex = -1;

};

}
