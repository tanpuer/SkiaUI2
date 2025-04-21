#pragma once

#include "ScrollView.h"

namespace HYSkiaUI {

class RecyclerView : public ScrollView {

#define DISTANCE 300

public:

    RecyclerView();

    virtual ~RecyclerView();

    virtual View *onCreateView(uint32_t index) = 0;

    virtual void onBindView(uint32_t index, View *view) = 0;

    void setDataSize(uint32_t size);

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

private:

    void initChildren();

    uint32_t size = 0;

    bool firstFlag = true;

    uint32_t firstChildIndex = 0;

    std::vector<View *> viewCache;

};

}
