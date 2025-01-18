#pragma once

#include "ScrollView.h"

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

class LyricScrollView : public ScrollView {

public:

    LyricScrollView();

    void setSourceSRT(const char *source);

    void draw(SkCanvas *canvas) override;

    void setCurrPositionFunc(std::function<long()> &&func);

    void pause();

    void start();

private:

    View *initItem(int index);

    std::vector<Lyric> result;

    long startTimeMills = 0L;

    std::function<long()> currentPositionFunc = nullptr;

    int currentIndex = -1;

    bool paused = false;

    bool lastIndexFlag = false;

};

}
