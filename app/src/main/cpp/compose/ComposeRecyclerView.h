#pragma once

#include "RecyclerView.h"

namespace HYSkiaUI {

class ComposeRecyclerView : public RecyclerView {

public:

    ComposeRecyclerView();

    ~ComposeRecyclerView();

    View *onCreateView(uint32_t index) override;

    void onBindView(uint32_t index, HYSkiaUI::View *view) override;

    uint32_t getViewType(uint32_t index) override;

    void checkJavaViewRef(jobject instance) override;

private:

    jmethodID createViewMethodId = nullptr;
    jmethodID bindViewMethodId = nullptr;
    jmethodID viewTypeMethodId = nullptr;

};

}
