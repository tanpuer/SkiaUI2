#pragma once

#include "View.h"
#include "string"
#include "core/SkImage.h"
#include "View.h"
#include "effects/SkRuntimeEffect.h"

namespace HYSkiaUI {

class YUVVideoView : public View {

public:

    YUVVideoView();

    ~YUVVideoView() override;

    void setSource(const char *path);

    const char *getSource();

    void start();

    void pause();

    void draw(SkCanvas *canvas) override;

    const char *name() override;

    void onShow() override;

    void onHide() override;

    void setRenderFirstFrameCallback(std::function<void()> &&callback);

private:

    jclass javaVideoClass = nullptr;

    jmethodID javaVideoConstructor = nullptr;

    jmethodID startMethod = nullptr;

    jmethodID pauseMethod = nullptr;

    jmethodID releaseMethod = nullptr;

    jmethodID getYUVDataMethod = nullptr;

    jobject javaVideo = nullptr;

    SkRect dstRect;

    std::string src;

    sk_sp<SkRuntimeEffect> runtimeEffect = nullptr;

    std::function<void()> renderFirstFrameCallback = nullptr;

    bool firstFrame = true;

};

}
