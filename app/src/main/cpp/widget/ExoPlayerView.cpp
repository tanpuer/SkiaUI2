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
    auto jniEnv = getContext()->getJniEnv();
    setSourceMethodId = jniEnv->GetMethodID(javaClass, "setSource",
                                            "(Ljava/lang/String;)V");
    auto jstring = jniEnv->NewStringUTF(this->source.c_str());
    jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jstring);
    jniEnv->DeleteLocalRef(jstring);
}

void ExoPlayerView::setSource(const char *source) {
    this->source = source;
    if (javaInstance != nullptr && setSourceMethodId != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        auto jstring = jniEnv->NewStringUTF(this->source.c_str());
        jniEnv->CallVoidMethod(javaInstance, setSourceMethodId, jstring);
        jniEnv->DeleteLocalRef(jstring);
    }
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

}
