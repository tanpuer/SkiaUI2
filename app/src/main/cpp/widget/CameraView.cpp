#include "CameraView.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"

namespace HYSkiaUI {


CameraView::CameraView() {

}

CameraView::~CameraView() {
    if (javaCamera != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaCamera, releaseMethod);
        jniEnv->DeleteGlobalRef(javaCamera);
        javaCamera = nullptr;
    }
}

const char *CameraView::name() {
    return View::name();
}

void CameraView::draw(SkCanvas *canvas) {
    markDirty();
    View::draw(canvas);
    auto env = getContext()->getJniEnv();
    auto yuvData = env->CallObjectMethod(javaCamera, getYUVDataMethod);
    if (yuvData == nullptr) {
        return;
    }
    static jclass yuvDataClass = env->GetObjectClass(yuvData);
    static jfieldID bufferYFiend = env->GetFieldID(yuvDataClass, "yData", "Ljava/nio/ByteBuffer;");
    static jfieldID bufferUFiend = env->GetFieldID(yuvDataClass, "uData", "Ljava/nio/ByteBuffer;");
    static jfieldID bufferVFiend = env->GetFieldID(yuvDataClass, "vData", "Ljava/nio/ByteBuffer;");
    static jfieldID strideYField = env->GetFieldID(yuvDataClass, "strideY", "I");
    static jfieldID strideUField = env->GetFieldID(yuvDataClass, "strideU", "I");
    static jfieldID strideVField = env->GetFieldID(yuvDataClass, "strideV", "I");
    static jfieldID widthField = env->GetFieldID(yuvDataClass, "width", "I");
    static jfieldID heightField = env->GetFieldID(yuvDataClass, "height", "I");
    static jfieldID rotationField = env->GetFieldID(yuvDataClass, "rotation", "I");
    uint8_t *y = static_cast<uint8_t *>(env->GetDirectBufferAddress(
            env->GetObjectField(yuvData, bufferYFiend)));
    uint8_t *u = static_cast<uint8_t *>(env->GetDirectBufferAddress(
            env->GetObjectField(yuvData, bufferUFiend)));
    uint8_t *v = static_cast<uint8_t *>(env->GetDirectBufferAddress(
            env->GetObjectField(yuvData, bufferVFiend)));
    jint strideY = env->GetIntField(yuvData, strideYField);
    jint strideU = env->GetIntField(yuvData, strideUField);
    jint strideV = env->GetIntField(yuvData, strideVField);
    jint width = env->GetIntField(yuvData, widthField);
    jint height = env->GetIntField(yuvData, heightField);
    jint rotation = env->GetIntField(yuvData, rotationField);
    int ySize = strideY * height;
    int uSize = (strideU * height) / 2;
    int vSize = (strideV * height) / 2;
    if (runtimeEffect != nullptr) {
        SkCanvas *skCanvas;
        SkPictureRecorder recorder;
        skCanvas = recorder.beginRecording(width, height);
        auto y_imageInfo = SkImageInfo::Make(strideY, height, kGray_8_SkColorType,
                                             kPremul_SkAlphaType);
        auto u_imageInfo = SkImageInfo::Make(strideU, height / 2, kGray_8_SkColorType,
                                             kPremul_SkAlphaType);
        auto v_imageInfo = SkImageInfo::Make(strideV, height / 2, kGray_8_SkColorType,
                                             kPremul_SkAlphaType);
        sk_sp<SkData> y_data = SkData::MakeWithCopy(y, ySize);
        sk_sp<SkData> u_data = SkData::MakeWithCopy(u, uSize);
        sk_sp<SkData> v_data = SkData::MakeWithCopy(v, vSize);
        auto y_image = SkImages::RasterFromData(y_imageInfo, y_data, strideY);
        auto u_image = SkImages::RasterFromData(u_imageInfo, u_data, strideU);
        auto v_image = SkImages::RasterFromData(v_imageInfo, v_data, strideV);
        SkRuntimeShaderBuilder builder(runtimeEffect);
        builder.child("y_tex") = y_image->makeShader(SkSamplingOptions());
        builder.child("u_tex") = u_image->makeShader(SkSamplingOptions());
        builder.child("v_tex") = v_image->makeShader(SkSamplingOptions());
        float widthRatio = this->width * 1.0f / width;
        float heightRatio = this->height * 1.0f / height;
        float ratio = std::min(widthRatio, heightRatio);
        builder.uniform("widthRatio") = ratio;
        builder.uniform("heightRatio") = ratio;
        builder.uniform("rotation") = SK_ScalarPI / 180.0f * rotation;
        sk_sp<SkShader> shader = builder.makeShader();
        SkPaint skPaint;
        skPaint.setAntiAlias(true);
        skPaint.setShader(std::move(shader));
        skCanvas->drawRect(SkRect::MakeXYWH(0, 0, width * ratio, height * ratio),
                           skPaint);
        auto picture = recorder.finishRecordingAsPicture();
        canvas->save();
//        canvas->translate(left, top);
        canvas->translate(left, top + (this->height - height *ratio) / 2);
        canvas->rotate(rotation, this->width / 2.0, height * ratio / 2.0);
        canvas->drawPicture(picture);
        canvas->restore();
    }
}

void CameraView::setContext(std::shared_ptr<SkiaUIContext> context) {
    View::setContext(context);
}

void CameraView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (!inited) {
        inited = true;
        auto jniEnv = context->getJniEnv();
        javaCameraClass = jniEnv->FindClass(
                "com/temple/skiaui/platform/camera/PlatformCameraViewPlugin");
        javaCameraConstructor = jniEnv->GetMethodID(javaCameraClass, "<init>",
                                                    "(Lcom/temple/skiaui/HYSkiaEngine;II)V");
        getYUVDataMethod = jniEnv->GetMethodID(javaCameraClass, "getYUVData",
                                               "()Lcom/temple/skiaui/platform/data/ImageReaderYUVData;");
        showMethod = jniEnv->GetMethodID(javaCameraClass, "onShow", "()V");
        hideMethod = jniEnv->GetMethodID(javaCameraClass, "onHide", "()V");
        releaseMethod = jniEnv->GetMethodID(javaCameraClass, "release", "()V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        javaCamera = jniEnv->NewGlobalRef(jniEnv->NewObject(javaCameraClass, javaCameraConstructor,
                                                            javaSkiaEngine, width, height));
        auto assetManager = getContext()->getAssetManager();
        const char *kYUVtoRGBShader = assetManager->readFile("skia_yuv420p_camera_shader.glsl");
        auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(kYUVtoRGBShader));
        if (!effect) {
            ALOGD("set shader source failed %s", error.data())
            return;
        }
        runtimeEffect = effect;
    }
}

void CameraView::onShow() {
    View::onShow();
    if (javaCamera != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaCamera, showMethod);
    }
}

void CameraView::onHide() {
    View::onHide();
    if (javaCamera != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaCamera, hideMethod);
    }
}

}
