#include "CameraView.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "core/SkColorSpace.h"

namespace HYSkiaUI {


CameraView::CameraView() {
}

CameraView::~CameraView() {
}

const char *CameraView::name() {
    return "Camera";
}

void CameraView::capture(std::function<void(sk_sp<SkImage>)> &&captureCallback) {
    this->captureCallback = std::move(captureCallback);
    captureInNextDraw = true;
}

const char *CameraView::getJavaClassPath() {
    return "com/temple/skiaui/platform/camera/PlatformCameraViewPlugin";
}

void CameraView::initJNI() {
    BaseSurfaceTextureView::initJNI();
    auto jniEnv = context->getJniEnv();
    startMethodId = jniEnv->GetMethodID(javaClass, "start", "()V");
    jniEnv->CallVoidMethod(javaInstance, startMethodId);
    getRotationMethodId = jniEnv->GetMethodID(javaClass, "getRotation", "()I");
    getCameraWidthMethodId = jniEnv->GetMethodID(javaClass, "getCameraWidth", "()I");
    getCameraHeightMethodId = jniEnv->GetMethodID(javaClass, "getCameraHeight", "()I");
}

void CameraView::layout(int l, int t, int r, int b) {
    BaseSurfaceTextureView::layout(l, t, r, b);
}

void CameraView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaInstance, getSkImageMethodId);
    if (skImagePtr != lastSkImagePtr) {
        if (skImagePtr != 0L) {
            auto image = reinterpret_cast<SkImage *>(skImagePtr);
            SkSafeUnref(skImage);
            skImage = SkSafeRef(image);
        } else {
            SkSafeUnref(skImage);
            skImage = nullptr;
        }
        lastSkImagePtr = skImagePtr;
        drawOneFrame();
    }
    auto rotation = jniEnv->CallIntMethod(javaInstance, getRotationMethodId);
    if (rotation < 0) {
        return;
    }
    auto cameraWidth = jniEnv->CallIntMethod(javaInstance, getCameraWidthMethodId);
    auto cameraHeight = jniEnv->CallIntMethod(javaInstance, getCameraHeightMethodId);
    cameraRatio =
            static_cast<float>(cameraWidth) / static_cast<float >(cameraHeight);
    viewRatio = static_cast<float>(width) / static_cast<float>(height);

    if (YGFloatsEqual(0.0f, cameraRatio)) {
        return;
    }
    auto recordingWidth = 0.0f;
    auto recordingHeight = 0.0f;
    if (cameraRatio < viewRatio) {
        recordingHeight = static_cast<float>(height);
        recordingWidth = static_cast<float>(height) * cameraRatio;
    } else {
        recordingWidth = static_cast<float>(width);
        recordingHeight = static_cast<float>(width) / cameraRatio;
    }
    SkCanvas *skCanvas;
    SkPictureRecorder recorder;
    skCanvas = recorder.beginRecording(recordingWidth, recordingHeight);
    SkRect target{0, 0, recordingWidth, recordingHeight};
    imageMatrix.setIdentity();
    if (rotation == 270) {
        imageMatrix.preScale(-1, 1, target.centerX(), 0);
//        imageMatrix.preRotate(270, target.centerX(), target.centerY());
//        if (cameraRatio > viewRatio) {
//            imageMatrix.postScale(1.0f, viewRatio / cameraRatio, target.centerX(),
//                                  target.centerY());
//        } else {
//            imageMatrix.postScale(cameraRatio / viewRatio, 1.0f, target.centerX(),
//                                  target.centerY());
//        }
    } else if (rotation == 90) {
        //back camera
    }
    skCanvas->setMatrix(imageMatrix);
    skCanvas->drawImageRect(skImage, target, SkSamplingOptions());
    auto picture = recorder.finishRecordingAsPicture();

    canvas->save();
    canvas->translate(left + (width - recordingWidth) / 2.0f, top + (height - recordingHeight) / 2.0f);
    canvas->drawPicture(picture);
    canvas->restore();
}

}
