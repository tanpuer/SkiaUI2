#include "CameraView.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "core/SkColorSpace.h"

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
    if (runtimeEffect == nullptr) {
        yuvFormat = strideY == strideU ? YUVFormat::NV12 : YUVFormat::YUV420;
        initShader(yuvFormat);
    }
    if (runtimeEffect != nullptr) {
        SkCanvas *skCanvas;
        SkPictureRecorder recorder;
        float widthRatio = this->width * 1.0f / width;
        float heightRatio = this->height * 1.0f / height;
        float ratio = std::min(widthRatio, heightRatio);
        skCanvas = recorder.beginRecording(width * ratio, height * ratio);
        SkRuntimeShaderBuilder builder(runtimeEffect);
        if (yuvFormat == YUVFormat::NV12) {
            int ySize = strideY * height;
            int uvSize = strideU * height / 2;
            auto y_imageInfo = SkImageInfo::Make(strideY, height, kGray_8_SkColorType,
                                                 kPremul_SkAlphaType);
            auto uv_imageInfo = SkImageInfo::Make(strideU / 2, height / 2, kR8G8_unorm_SkColorType,
                                                  kPremul_SkAlphaType);
            sk_sp<SkData> y_data = SkData::MakeWithCopy(y, ySize);
            sk_sp<SkData> uv_data = SkData::MakeWithCopy(u, uvSize);
            if (!uv_data) {
                ALOGD("Failed to create UV data copy");
                return;
            }
            auto y_image = SkImages::RasterFromData(y_imageInfo, y_data, strideY);
            if (!y_image) {
                ALOGD("Failed to create Y texture");
                return;
            }
            auto uv_image = SkImages::RasterFromData(uv_imageInfo, uv_data, strideU);
            if (!uv_image) {
                ALOGD("Failed to create UV texture. Possible reasons:");
                ALOGD("1. Stride alignment: %d", strideU);
                ALOGD("2. Required size: %zu, Actual size: %zu",
                      uv_imageInfo.computeMinByteSize(),
                      uv_data->size());
                return;
            }
            builder.child("y_tex") = y_image->makeShader(SkSamplingOptions());
            builder.child("uv_tex") = uv_image->makeShader(SkSamplingOptions());
        } else if (yuvFormat == YUVFormat::YUV420) {
            int ySize = strideY * height;
            int uSize = (strideU * height) / 2;
            int vSize = (strideV * height) / 2;
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
            builder.child("y_tex") = y_image->makeShader(SkSamplingOptions());
            builder.child("u_tex") = u_image->makeShader(SkSamplingOptions());
            builder.child("v_tex") = v_image->makeShader(SkSamplingOptions());
        }
        builder.uniform("widthRatio") = 1.0f;
        builder.uniform("heightRatio") = 1.0f;
        builder.uniform("rotation") = SK_ScalarPI / 180.0f * rotation;
        sk_sp<SkShader> shader = builder.makeShader();
        SkPaint skPaint;
        skPaint.setAntiAlias(true);
        skPaint.setShader(std::move(shader));
        skCanvas->drawRect(SkRect::MakeXYWH(0, 0, width * ratio, height * ratio),
                           skPaint);
        auto picture = recorder.finishRecordingAsPicture();
        if (captureInNextDraw) {
            captureInNextDraw = false;
            sk_sp<SkImage> skImage = SkImages::DeferredFromPicture(
                    picture,
                    SkISize::Make(width * ratio, height * ratio),
                    nullptr,
                    nullptr,
                    SkImages::BitDepth::kU8, SkColorSpace::MakeSRGB()
            );
            if (captureCallback != nullptr) {
                captureCallback(skImage);
            }
        }
        canvas->save();
        canvas->translate(left, top + (this->height - height * ratio) / 2);
        canvas->rotate(rotation, this->width / 2.0, height * ratio / 2.0);
        canvas->drawPicture(picture);
        canvas->restore();
    }
}

void CameraView::setContext(std::shared_ptr<SkiaUIContext>& context) {
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

void CameraView::initShader(YUVFormat format) {
    auto assetManager = getContext()->getAssetManager();
    std::string shaderPath;
    if (format == YUVFormat::YUV420) {
        shaderPath = "skia_yuv420p_camera_shader.glsl";
    } else if (format == YUVFormat::NV12) {
        shaderPath = "skia_nv12_fragment_shader.glsl";
    }
    const char *kYUVtoRGBShader = assetManager->readFile(shaderPath.c_str());
    auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(kYUVtoRGBShader));
    if (!effect) {
        ALOGD("set shader source failed %s", error.data())
        return;
    }
    runtimeEffect = effect;
}

void CameraView::capture(std::function<void(sk_sp<SkImage>)> &&captureCallback) {
    this->captureCallback = std::move(captureCallback);
    captureInNextDraw = true;
}

}
