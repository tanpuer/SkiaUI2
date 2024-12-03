#pragma once

#include "Page.h"

namespace HYSkiaUI {

static bool blackWhiteMode = false;

class ExamplePage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

    void drawOnFrame(int drawCount) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

};

}