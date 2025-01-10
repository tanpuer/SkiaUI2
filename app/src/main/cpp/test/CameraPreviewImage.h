#pragma once

#include "View.h"

namespace HYSkiaUI {

class CameraPreviewImage : public View {

public:

    CameraPreviewImage();

    ~CameraPreviewImage();

    void setImage(sk_sp<SkImage> image);

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void setRotateZ(int rotateZ);

private:

    SkRect dstRect;

    sk_sp<SkImage> skImage = nullptr;

    SkMatrix imageMatrix;

    std::unique_ptr<SkPaint> imagePaint;

};

}
