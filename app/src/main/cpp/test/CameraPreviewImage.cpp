#include "CameraPreviewImage.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"

namespace HYSkiaUI {

CameraPreviewImage::CameraPreviewImage() {
    imageMatrix = SkMatrix::I();
    imagePaint = std::make_unique<SkPaint>();
    imagePaint->setAntiAlias(true);
}

CameraPreviewImage::~CameraPreviewImage() {

}

void CameraPreviewImage::setImage(sk_sp<SkImage> image) {
    this->skImage = image;
    markDirty();
}

void CameraPreviewImage::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (skImage == nullptr) {
        return;
    }
    SkCanvas *skCanvas;
    SkPictureRecorder recorder;
    skCanvas = recorder.beginRecording(height, width);
    imageMatrix.setIdentity();
    imageMatrix.preRotate(rotateZ, height / 2.0, width / 2.0);
    imageMatrix.preScale(height * 1.0f / skImage->width(), width * 1.0f / skImage->height());
    skCanvas->setMatrix(imageMatrix);
    skCanvas->drawImage(skImage, 0, 0);
    auto picture = recorder.finishRecordingAsPicture();
    canvas->save();
    auto diff = (height - width) / 2.0f;
    canvas->translate(left - diff, top + diff);
    canvas->drawPicture(picture);
    canvas->restore();
}

void CameraPreviewImage::setRotateZ(int rotateZ) {
    this->rotateZ = rotateZ;
    markDirty();
}

void CameraPreviewImage::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(l, t, r, b);
}

}
