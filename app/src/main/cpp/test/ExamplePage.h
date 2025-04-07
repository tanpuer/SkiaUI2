#pragma once

#include "Page.h"
#include "LinearAnimator.h"
#include "FlexboxLayout.h"

namespace HYSkiaUI {

static bool blackWhiteMode = false;

class ExamplePage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

    void onShow() override;

    void onHide() override;

private:

    void initChildren(ViewGroup *root, int width, int height);

    LinearAnimator *lottieAnimator = nullptr;

    LinearAnimator *textAnimator = nullptr;

    void pushPage(int width, int height);

    void popPage(int width, int height);

};

}