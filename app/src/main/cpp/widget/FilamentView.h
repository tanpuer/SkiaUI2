#pragma once

#include "BaseSurfaceTextureView.h"

namespace HYSkiaUI {

class FilamentView : public BaseSurfaceTextureView {

public:

    FilamentView();

    ~FilamentView() override;

    const char * getJavaClassPath() override;

};

}
