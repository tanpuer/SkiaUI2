#include "ExoPlayerView.h"

namespace HYSkiaUI {


ExoPlayerView::ExoPlayerView() {
}

ExoPlayerView::~ExoPlayerView() {
}

const char *ExoPlayerView::getJavaClassPath() {
    return "com/temple/skiaui/platform/video/PlatformVideoViewPlugin";
}

void ExoPlayerView::initJNI() {
    BaseSurfaceTextureView::initJNI();
    auto jniEnv = context->getJniEnv();
    setSourceMethodId = jniEnv->GetMethodID(javaClass, "setSource",
                                            "(Ljava/lang/String;)V");
    auto jstring = jniEnv->NewStringUTF(this->source.c_str());
    jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jstring);
    jniEnv->DeleteLocalRef(jstring);
}

void ExoPlayerView::setSource(const char *source) {
    this->source = source;
    if (javaInstance != nullptr && setSourceMethodId != nullptr) {
        auto jniEnv = context->getJniEnv();
        auto jstring = jniEnv->NewStringUTF(this->source.c_str());
        jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jstring);
        jniEnv->DeleteLocalRef(jstring);
    }
}

void ExoPlayerView::setCustomVideoPlayer(jobject player) {
    auto jniEnv = context->getJniEnv();
    if (javaInstance == nullptr) {
        javaClass = jniEnv->FindClass(getJavaClassPath());
        javaConstructor = jniEnv->GetMethodID(javaClass, "<init>",
                                              "(Lcom/temple/skiaui/HYSkiaEngine;IIJ)V");
        auto javaSkiaEngine = context->getJavaSkiaEngine();
        javaInstance = jniEnv->NewGlobalRef(
                jniEnv->NewObject(javaClass, javaConstructor,
                                  javaSkiaEngine, width, height, reinterpret_cast<long>(this)));
    }
    setCustomPlayerMethodId = jniEnv->GetMethodID(
            javaClass, "setCustomPlayer", "(Lcom/temple/skiaui/platform/video/IVideoPlayer;)V");
    jniEnv->CallVoidMethod(javaInstance, setCustomPlayerMethodId, player);
}

void ExoPlayerView::setRenderFirstFrameCallback(std::function<void()> &&callback) {
    this->renderFirstFrameCallback = std::move(callback);
}

void ExoPlayerView::drawOneFrame() {
    if (firstFrame) {
        firstFrame = false;
        context->setTimer([this]() {
            if (renderFirstFrameCallback != nullptr) {
                renderFirstFrameCallback();
            }
        }, 0, false);
    }
}

const char *ExoPlayerView::getSource() {
    return source.c_str();
}

const char *ExoPlayerView::name() {
    return "ExoPlayerView";
}

void ExoPlayerView::onVideoSizeChanged(int width, int height) {

}

}
