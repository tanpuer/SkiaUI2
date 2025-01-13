#pragma once

#include "View.h"

namespace HYSkiaUI {

class CameraPreviewImage : public View {

public:

    CameraPreviewImage();

    ~CameraPreviewImage();

    void setImage(sk_sp<SkImage> image);

    void draw(SkCanvas *canvas) override;

    void setRotateZ(int rotateZ);

private:

    sk_sp<SkImage> skImage = nullptr;

    SkMatrix imageMatrix;

    std::unique_ptr<SkPaint> imagePaint;

};

}
