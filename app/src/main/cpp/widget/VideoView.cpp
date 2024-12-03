#include "VideoView.h"
#include "jni.h"

namespace HYSkiaUI {

VideoView::VideoView() {
    videoPaint = std::make_unique<SkPaint>();
    videoPaint->setAntiAlias(true);
}

VideoView::~VideoView() {
    auto jniEnv = getContext()->getJniEnv();
    if (lastSkImagePtr != 0L) {
        jniEnv->CallVoidMethod(javaVideo, deleteSkImageMethod, lastSkImagePtr);
    }
    jniEnv->CallVoidMethod(javaVideo, releaseMethod);
    jniEnv->DeleteGlobalRef(javaVideo);
}

void VideoView::setSource(const char *path) {
    MeasureTime measureTime("VideoView setSource");
    src = std::string(path);
    auto jniEnv = getContext()->getJniEnv();
    javaVideoClass = jniEnv->FindClass("com/temple/skiaui/video/HYSkiaVideo");
    javaVideoConstructor = jniEnv->GetMethodID(javaVideoClass, "<init>",
                                               "(Ljava/lang/String;Lcom/temple/skiaui/HYSkiaEngine;)V");
    getCurrentSkImage = jniEnv->GetMethodID(javaVideoClass, "getCurrentSkImage", "()J");
    startMethod = jniEnv->GetMethodID(javaVideoClass, "start", "()V");
    pauseMethod = jniEnv->GetMethodID(javaVideoClass, "pause", "()V");
    releaseMethod = jniEnv->GetMethodID(javaVideoClass, "release", "()V");
    deleteSkImageMethod = jniEnv->GetMethodID(javaVideoClass, "deleteSkImage", "(J)V");
    auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
    javaVideo = jniEnv->NewGlobalRef(jniEnv->NewObject(javaVideoClass, javaVideoConstructor,
                                                       jniEnv->NewStringUTF(path), javaSkiaEngine));
}

const char *VideoView::getSource() {
    return src.c_str();
}

void VideoView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
}

void VideoView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaVideo, getCurrentSkImage);
    if (skImagePtr != lastSkImagePtr) {
//        jniEnv->CallVoidMethod(javaVideo, deleteSkImageMethod, lastSkImagePtr);
        if (skImagePtr != 0L) {
            auto image = reinterpret_cast<SkImage *>(skImagePtr);
            skImage = SkSafeRef(image);
        } else {
            skImage = nullptr;
        }
        lastSkImagePtr = skImagePtr;
    }
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), videoPaint.get());
}

void VideoView::start() {

}

void VideoView::pause() {

}

void VideoView::onShow() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaVideo, startMethod);
}

void VideoView::onHide() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaVideo, pauseMethod);
}

const char *VideoView::name() {
    return "VideoView";
}

}
