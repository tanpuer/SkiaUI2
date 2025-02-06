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

    void setRenderFirstFrameCallback(std::function<void()> &&callback);

private:

    jmethodID setSourceMethodId = nullptr;

    std::string source;

    bool firstFrame = true;

    std::function<void()> renderFirstFrameCallback = nullptr;

};

}
