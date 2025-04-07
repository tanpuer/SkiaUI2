#pragma once

#include "BaseSurfaceTextureView.h"

namespace HYSkiaUI {

class CameraView : public BaseSurfaceTextureView {

public:

    CameraView();

    ~CameraView();

    const char *getJavaClassPath() override;

    void initJNI() override;

    const char *name() override;

    void capture(std::function<void(sk_sp<SkImage>)> &&captureCallback);

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void switchCamera();

private:

    SkMatrix imageMatrix;

    SkRect srcRect;

    std::function<void(sk_sp<SkImage>)> captureCallback;

    bool captureInNextDraw = false;

    jmethodID startMethodId = nullptr;

    jmethodID getRotationMethodId = nullptr;

    jmethodID getCameraWidthMethodId = nullptr;

    jmethodID getCameraHeightMethodId = nullptr;

    jmethodID switchCameraMethodId = nullptr;

    float cameraRatio = 0.0f;

    float viewRatio = 0.0f;

};

}
