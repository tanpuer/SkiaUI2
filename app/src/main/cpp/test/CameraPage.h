#pragma once

#include "Page.h"
#include "ImageView.h"
#include "CameraView.h"

namespace HYSkiaUI {

class CameraPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

    CameraView *cameraView = nullptr;

};

}
