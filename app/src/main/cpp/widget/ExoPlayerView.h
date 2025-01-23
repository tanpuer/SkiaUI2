#pragma once

#include "View.h"

namespace HYSkiaUI {

class ExoPlayerView : public View {

public:

    ExoPlayerView();

    ~ExoPlayerView();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void onShow() override;

    void onHide() override;

    void setSource(const char* source);

    void setRenderFirstFrameCallback(std::function<void()> &&callback);

private:

    SkRect dstRect;

    bool inited = false;

    jclass javaExoPlayerClass = nullptr;

    jmethodID javaExoConstructor = nullptr;

    jobject javaExoPlayer = nullptr;

    jmethodID getSkImageMethodId = nullptr;

    long lastSkImagePtr = 0L;

    SkImage *skImage = nullptr;

    std::unique_ptr<SkPaint> platformPaint;

    jmethodID releaseMethod = nullptr;

    jmethodID showMethod = nullptr;

    jmethodID hideMethod = nullptr;

    jmethodID setSourceMethodId = nullptr;

    std::string source;

    bool firstFrame = true;

    std::function<void()> renderFirstFrameCallback = nullptr;

};

}
