#pragma once

#include "View.h"
#include "string"
#include "core/SkImage.h"
#include "View.h"
#include "effects/SkRuntimeEffect.h"

class YUVVideoView : public View {

public:

    YUVVideoView();

    virtual ~YUVVideoView();

    void setSource(const char *path);

    const char* getSource();

    void start();

    void pause();

    void draw(SkCanvas *canvas) override;

    const char *name() override;

    void onShow() override;

    void onHide() override;

private:

    jclass javaVideoClass = nullptr;

    jmethodID javaVideoConstructor = nullptr;

    jmethodID startMethod = nullptr;

    jmethodID pauseMethod = nullptr;

    jmethodID releaseMethod = nullptr;

    jmethodID deleteSkImageMethod = nullptr;

    jmethodID getYUVDataMethod = nullptr;

    jobject javaVideo = nullptr;

    SkRect dstRect;

    std::unique_ptr<SkPaint> videoPaint;

    long lastSkImagePtr = 0L;

    std::string src;

    sk_sp<SkRuntimeEffect> runtimeEffect = nullptr;

};