#pragma once

#include "BaseSurfaceTextureView.h"

namespace HYSkiaUI {

class ExoPlayerView : public BaseSurfaceTextureView {

public:

    ExoPlayerView();

    ~ExoPlayerView() override;

    const char *getJavaClassPath() override;

    void initJNI() override;

    void drawOneFrame() override;

    void setSource(const char *source);

    const char *getSource();

    const char *name() override;

    void setRenderFirstFrameCallback(std::function<void()> &&callback);

    void setCustomVideoPlayer(jobject player);

    void setBackgroundPlayback(bool flag);

    void setRepeatMode(bool repeatable);

private:

    jmethodID setSourceMethodId = nullptr;

    jmethodID setCustomPlayerMethodId = nullptr;

    jmethodID setBackgroundPlaybackMethodId = nullptr;

    jmethodID setRepeatModeMethodId = nullptr;

    std::string source;

    bool firstFrame = true;

    std::function<void()> renderFirstFrameCallback = nullptr;

};

}
