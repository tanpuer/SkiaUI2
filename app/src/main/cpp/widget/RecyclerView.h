#pragma once

#include "ScrollView.h"
#include "LinearAnimator.h"

namespace HYSkiaUI {

class RecyclerView : public ScrollView {

#define DISTANCE 300
#define SMOOTH_DISTANCE DISTANCE / 2

public:

    RecyclerView();

    virtual ~RecyclerView();

    virtual View *onCreateView(uint32_t index) = 0;

    virtual void onBindView(uint32_t index, View *view) = 0;

    virtual int getViewType(uint32_t index) = 0;

    void setDataSize(uint32_t size);

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    const char *name() override;

    virtual void scrollToPosition(int position);

    virtual void smoothScrollToPosition(int position);

    void updateTranslateY(float diffY) override;

    bool isSmoothScrolling();

private:

    void initChildren();

    uint32_t size = 0;

    bool firstFlag = true;

    uint32_t firstChildIndex = 0;

    std::unordered_map<int, std::vector<View *>> viewCache;

    View *getViewFromCache(int index);

    void putViewToCache(int index, View *view);

    void layoutNewAddedChild(int l, int t, int r, int b, View *view);

    LinearAnimator *smoothAnimator = nullptr;

};

}
