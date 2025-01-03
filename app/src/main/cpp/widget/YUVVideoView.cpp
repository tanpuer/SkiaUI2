#include "YUVVideoView.h"
#include "jni.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"

namespace HYSkiaUI {

YUVVideoView::YUVVideoView() {
}

YUVVideoView::~YUVVideoView() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaVideo, releaseMethod);
    jniEnv->DeleteGlobalRef(javaVideo);
}

void YUVVideoView::setSource(const char *path) {
    MeasureTime measureTime("VideoView setSource");
    src = std::string(path);
    auto jniEnv = getContext()->getJniEnv();
    javaVideoClass = jniEnv->FindClass("com/temple/skiaui/video/HYSkiaYUVVideo");
    javaVideoConstructor = jniEnv->GetMethodID(javaVideoClass, "<init>",
                                               "(Ljava/lang/String;Lcom/temple/skiaui/HYSkiaEngine;)V");
    startMethod = jniEnv->GetMethodID(javaVideoClass, "start", "()V");
    pauseMethod = jniEnv->GetMethodID(javaVideoClass, "pause", "()V");
    releaseMethod = jniEnv->GetMethodID(javaVideoClass, "release", "()V");
    getYUVDataMethod = jniEnv->GetMethodID(javaVideoClass, "getYUVData",
                                           "()Lcom/temple/skiaui/video/YUVData;");
    auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
    javaVideo = jniEnv->NewGlobalRef(jniEnv->NewObject(javaVideoClass, javaVideoConstructor,
                                                       jniEnv->NewStringUTF(path), javaSkiaEngine));
    auto assetManager = getContext()->getAssetManager();
    const char *kYUVtoRGBShader = assetManager->readFile("skia_yuv420p_fragment_shader.glsl");
    auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(kYUVtoRGBShader));
    if (!effect) {
        ALOGD("set shader source failed %s", error.data())
        return;
    }
    runtimeEffect = effect;
    firstFrame = true;
}

const char *YUVVideoView::getSource() {
    return src.c_str();
}

void YUVVideoView::draw(SkCanvas *canvas) {
    if (getYUVDataMethod == nullptr) {
        return;
    }
    auto env = getContext()->getJniEnv();
    auto yuvData = env->CallObjectMethod(javaVideo, getYUVDataMethod);
    if (yuvData == nullptr) {
        return;
    }
    static jclass yuvDataClass = env->GetObjectClass(yuvData);
    static jfieldID bufferFiend = env->GetFieldID(yuvDataClass, "data", "Ljava/nio/ByteBuffer;");
    static jfieldID strideField = env->GetFieldID(yuvDataClass, "stride", "I");
    static jfieldID widthField = env->GetFieldID(yuvDataClass, "videoWidth", "I");
    static jfieldID heightField = env->GetFieldID(yuvDataClass, "videoHeight", "I");
    jobject buffer = env->GetObjectField(yuvData, bufferFiend);
    uint8_t *yuv = static_cast<uint8_t *>(env->GetDirectBufferAddress(buffer));
    jint stride = env->GetIntField(yuvData, strideField);
    jint width = env->GetIntField(yuvData, widthField);
    jint height = env->GetIntField(yuvData, heightField);
    int ySize = width * height;
    int uvSize = (width * height) / 4;
    if (runtimeEffect != nullptr) {
        SkCanvas *skCanvas;
        SkPictureRecorder recorder;
        skCanvas = recorder.beginRecording(width, height);
        auto y_imageInfo = SkImageInfo::Make(stride, height, SkColorType::kGray_8_SkColorType,
                                             kPremul_SkAlphaType);
        auto u_imageInfo = SkImageInfo::Make(stride / 2, height / 2, kGray_8_SkColorType,
                                             kPremul_SkAlphaType);
        auto v_imageInfo = SkImageInfo::Make(stride / 2, height / 2, kGray_8_SkColorType,
                                             kPremul_SkAlphaType);
        sk_sp<SkData> y_data = SkData::MakeWithCopy(yuv, ySize);
        sk_sp<SkData> u_data = SkData::MakeWithCopy(yuv + ySize, uvSize);
        sk_sp<SkData> v_data = SkData::MakeWithCopy(yuv + ySize + uvSize, uvSize);
        auto y_image = SkImages::RasterFromData(y_imageInfo, y_data, stride);
        auto u_image = SkImages::RasterFromData(u_imageInfo, u_data, stride / 2);
        auto v_image = SkImages::RasterFromData(v_imageInfo, v_data, stride / 2);
        SkRuntimeShaderBuilder builder(runtimeEffect);
        builder.child("y_tex") = y_image->makeShader(SkSamplingOptions());
        builder.child("u_tex") = u_image->makeShader(SkSamplingOptions());
        builder.child("v_tex") = v_image->makeShader(SkSamplingOptions());
        float widthRatio = this->width * 1.0f / width;
        float heightRatio = this->height * 1.0f / height;
        float ratio = std::min(widthRatio, heightRatio);
        builder.uniform("widthRatio") = ratio;
        builder.uniform("heightRatio") = ratio;
        sk_sp<SkShader> shader = builder.makeShader();
        SkPaint skPaint;
        skPaint.setAntiAlias(true);
        skPaint.setShader(std::move(shader));
        skCanvas->drawRect(SkRect::MakeXYWH(0, 0, width * ratio, height * ratio),
                           skPaint);
        auto picture = recorder.finishRecordingAsPicture();
        canvas->save();
        canvas->translate(left, top);
        canvas->drawPicture(picture);
        canvas->restore();
        if (firstFrame) {
            firstFrame = false;
            if (renderFirstFrameCallback != nullptr) {
                renderFirstFrameCallback();
            }
        }
        markDirty();
    }
}

void YUVVideoView::start() {

}

void YUVVideoView::pause() {

}

void YUVVideoView::onShow() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaVideo, startMethod);
}

void YUVVideoView::onHide() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaVideo, pauseMethod);
}

const char *YUVVideoView::name() {
    return "YUVVideoView";
}

void YUVVideoView::setRenderFirstFrameCallback(std::function<void()> &&callback) {
    this->renderFirstFrameCallback = std::move(callback);
}

}
