#pragma once

#include "ScrollView.h"
#include "LinearAnimator.h"

namespace HYSkiaUI {

class RecyclerView : public ScrollView {

#define DISTANCE 300.0f
#define SMOOTH_DISTANCE (DISTANCE / 2)

public:

    RecyclerView();

    ~RecyclerView() override;

    virtual View *onCreateView(uint32_t index) = 0;

    virtual void onBindView(uint32_t index, View *view) = 0;

    virtual uint32_t getViewType(uint32_t index) = 0;

    void setDataSize(uint32_t size);

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    const char *name() override;

    virtual void scrollToPosition(uint32_t position);

    virtual void smoothScrollToPosition(uint32_t position);

    void updateTranslateY(float diffY) override;

    bool isSmoothScrolling();

    void onShow() override;

    void onHide() override;

private:

    void initChildren();

    uint32_t size = 0;

    bool firstFlag = true;

    uint32_t firstChildIndex = 0;

    std::unordered_map<uint32_t, std::vector<View *>> viewCache;

    View *getViewFromCache(uint32_t index);

    void putViewToCache(uint32_t index, View *view);

    void layoutNewAddedChild(int l, int t, int r, int b, View *view);

    LinearAnimator *smoothAnimator = nullptr;

    bool viewShow = true;
};

}
