#include "CameraView.h"

namespace HYSkiaUI {


CameraView::CameraView() {

}

CameraView::~CameraView() {

}

const char *CameraView::getJavaPlatformViewName() {
    return "com/temple/skiaui/platform/camera/PlatformCameraViewPlugin";
}
}
