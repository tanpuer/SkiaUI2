#pragma once

#include "BaseSurfaceTextureView.h"

namespace HYSkiaUI {

class ExoPlayerView : public BaseSurfaceTextureView {

public:

    ExoPlayerView();

    ~ExoPlayerView();

    const char *getJavaClassPath() override;

    void initJNI() override;

    void drawOneFrame() override;

    void setSource(const char *source);

    const char *getSource();

    const char *name() override;

    void setRenderFirstFrameCallback(std::function<void()> &&callback);

    void onVideoSizeChanged(int width, int height);

    void setCustomVideoPlayer(jobject player);

private:

    jmethodID setSourceMethodId = nullptr;

    jmethodID setCustomPlayerMethodId = nullptr;

    std::string source;

    bool firstFrame = true;

    std::function<void()> renderFirstFrameCallback = nullptr;

};

}
