#pragma once

#include "PlatformView.h"

namespace HYSkiaUI {

class CameraView : public PlatformView {

public:

    CameraView();

    ~CameraView();
    
    const char *getJavaPlatformViewName() override;

};

}
