#include "ExoPlayerView.h"

namespace HYSkiaUI {


ExoPlayerView::ExoPlayerView() {
    platformPaint = std::make_unique<SkPaint>();
    platformPaint->setAntiAlias(true);
}

ExoPlayerView::~ExoPlayerView() {
    if (javaExoPlayer != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaExoPlayer, releaseMethod);
        jniEnv->DeleteGlobalRef(javaExoPlayer);
        javaExoPlayer = nullptr;
    }
}

void ExoPlayerView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
    if (!inited) {
        inited = true;
        auto jniEnv = context->getJniEnv();
        javaExoPlayerClass = jniEnv->FindClass(
                "com/temple/skiaui/platform/video/PlatformVideoViewPlugin");
        javaExoConstructor = jniEnv->GetMethodID(javaExoPlayerClass, "<init>",
                                                 "(Lcom/temple/skiaui/HYSkiaEngine;IIJ)V");
        getSkImageMethodId = jniEnv->GetMethodID(javaExoPlayerClass, "getSkImage", "()J");
        showMethod = jniEnv->GetMethodID(javaExoPlayerClass, "onShow", "()V");
        hideMethod = jniEnv->GetMethodID(javaExoPlayerClass, "onHide", "()V");
        releaseMethod = jniEnv->GetMethodID(javaExoPlayerClass, "release", "()V");
        setSourceMethodId = jniEnv->GetMethodID(javaExoPlayerClass, "setSource",
                                                "(Ljava/lang/String;)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        javaExoPlayer = jniEnv->NewGlobalRef(
                jniEnv->NewObject(javaExoPlayerClass, javaExoConstructor,
                                  javaSkiaEngine, width, height, reinterpret_cast<long>(this)));
        auto jstring = jniEnv->NewStringUTF(this->source.c_str());
        jniEnv->CallVoidMethod(javaExoPlayer, setSourceMethodId, jstring);
    }
}

void ExoPlayerView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaExoPlayer, getSkImageMethodId);
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
        if (firstFrame) {
            firstFrame = false;
            if (renderFirstFrameCallback != nullptr) {
                renderFirstFrameCallback();
            }
        }
    }
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), platformPaint.get());
}

void ExoPlayerView::onShow() {
    View::onShow();
    if (javaExoPlayer != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaExoPlayer, showMethod);
    }
}

void ExoPlayerView::onHide() {
    View::onHide();
    if (javaExoPlayer != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaExoPlayer, hideMethod);
    }
}

void ExoPlayerView::setSource(const char *source) {
    this->source = source;
}

void ExoPlayerView::setRenderFirstFrameCallback(std::function<void()> &&callback) {
    this->renderFirstFrameCallback = std::move(callback);
}

}
