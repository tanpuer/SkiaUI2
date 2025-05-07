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
    switchCameraMethodId = jniEnv->GetMethodID(javaClass, "switchCamera", "()V");
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
    recordingHeight = static_cast<float>(height);
    recordingWidth = static_cast<float>(height) * cameraRatio;
    SkCanvas *skCanvas;
    SkPictureRecorder recorder;
    skCanvas = recorder.beginRecording(recordingWidth, recordingHeight);
    SkRect target{0, 0, recordingWidth, recordingHeight};
    imageMatrix.setIdentity();
    if (rotation == 270) {
        //front camera
        imageMatrix.preScale(-1, 1, target.centerX(), 0);
        imageMatrix.preRotate(270, target.centerX(), target.centerY());
        auto rotatedWidth = recordingHeight;
        auto rotatedHeight = recordingWidth;
        auto scale = std::min(width / rotatedWidth, height / rotatedHeight);
        imageMatrix.preScale(scale, scale, target.centerX(), target.centerY());
    } else if (rotation == 90) {
        //back camera
        imageMatrix.preRotate(90, target.centerX(), target.centerY());
        auto rotatedWidth = recordingHeight;
        auto rotatedHeight = recordingWidth;
        auto scale = std::min(width / rotatedWidth, height / rotatedHeight);
        imageMatrix.preScale(scale, scale, target.centerX(), target.centerY());
    }
    skCanvas->setMatrix(imageMatrix);
    skCanvas->drawImageRect(skImage, target, SkSamplingOptions());
    auto picture = recorder.finishRecordingAsPicture();

    //TODO runtimeEffect should perform on origin skImage
    if (runtimeEffect != nullptr) {
        SkCanvas *skCanvas;
        SkPictureRecorder recorder;
        skCanvas = recorder.beginRecording(width, height);
        ResolutionUniforms uniforms;
        uniforms.width = width;
        uniforms.height = height;
        SkRuntimeShaderBuilder builder(runtimeEffect);
        builder.uniform("iResolution") = uniforms;
        auto time = getContext()->getCurrentTimeMills();
        builder.uniform("iTime") = (float) time / 1000;
        auto skShader = picture->makeShader(SkTileMode::kClamp, SkTileMode::kClamp, SkFilterMode::kLinear);
        builder.child("iChannel0") = std::move(skShader);
        auto shader = builder.makeShader(nullptr);
        SkPaint skPaint;
        skPaint.setShader(std::move(shader));
        skCanvas->drawIRect({0, 0, (int32_t)recordingWidth, (int32_t)recordingHeight}, skPaint);
        auto picture = recorder.finishRecordingAsPicture();
        canvas->setMatrix(viewMatrix);
        canvas->save();
        canvas->translate(left + (width - recordingWidth) / 2.0f,
                          top + (height - recordingHeight) / 2.0f);
        canvas->drawPicture(picture);
        canvas->restore();
        return;
    }

    canvas->setMatrix(viewMatrix);
    canvas->save();
    canvas->translate(left + (width - recordingWidth) / 2.0f,
                      top + (height - recordingHeight) / 2.0f);
    canvas->drawPicture(picture);
    canvas->restore();
}

void CameraView::switchCamera() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaInstance, switchCameraMethodId);
}

}
