#include "VideoView.h"
#include "jni.h"

VideoView::VideoView() {
    videoPaint = std::make_unique<SkPaint>();
    videoPaint->setAntiAlias(true);
}

VideoView::~VideoView() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaVideo, release);
    jniEnv->DeleteGlobalRef(javaVideo);
}

void VideoView::setSource(const char *path) {
    auto jniEnv = getContext()->getJniEnv();
    javaVideoClass = jniEnv->FindClass("com/temple/skiaui/video/HYSkiaVideo");
    javaVideoConstructor = jniEnv->GetMethodID(javaVideoClass, "<init>",
                                               "(Ljava/lang/String;Lcom/temple/skiaui/HYSkiaEngine;)V");
    getCurrentSkImage = jniEnv->GetMethodID(javaVideoClass, "getCurrentSkImage", "()J");
    release = jniEnv->GetMethodID(javaVideoClass, "release", "()V");
    auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
    javaVideo = jniEnv->NewGlobalRef(jniEnv->NewObject(javaVideoClass, javaVideoConstructor,
                                                       jniEnv->NewStringUTF(path), javaSkiaEngine));
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
        auto image = reinterpret_cast<SkImage *>(skImagePtr);
        skImage = sk_sp<SkImage>(SkSafeRef(image));
        lastSkImagePtr = skImagePtr;
    }
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), videoPaint.get());
}

void VideoView::start() {

}

void VideoView::pause() {

}
