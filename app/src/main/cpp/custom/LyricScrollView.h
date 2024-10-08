#pragma once

#include "ScrollView.h"
#include "LyricView.h"

class LyricScrollView : public ScrollView {

public:

    LyricScrollView();

    void setSourceSRT(const char *source);

    void draw(SkCanvas *canvas) override;

    void setCurrPositionFunc(std::function<long()>&& func);

private:

    View* initItem(int index);

    std::vector<Lyric> result;

    long startTimeMills = 0L;

    std::function<long()> currentPositionFunc = nullptr;

    int currentIndex = -1;

};
