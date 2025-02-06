#pragma once

#include "BaseSurfaceTextureView.h"

namespace HYSkiaUI {

class FilamentView : public BaseSurfaceTextureView {

public:

    FilamentView();

    ~FilamentView();

    const char * getJavaClassPath() override;

};

}
