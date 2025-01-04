#pragma once

#include "Page.h"

namespace HYSkiaUI {

class CameraPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

};

}
