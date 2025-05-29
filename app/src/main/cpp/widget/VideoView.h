#pragma once

#include "View.h"
#include "string"
#include "core/SkImage.h"
#include "View.h"

namespace HYSkiaUI {

class VideoView : public View {

public:

    VideoView();

    ~VideoView() override;

    void setSource(const char *path);

    const char *getSource();

    void start();

    void pause();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    const char *name() override;

    void onShow() override;

    void onHide() override;

private:

    jclass javaVideoClass = nullptr;

    jmethodID javaVideoConstructor = nullptr;

    jmethodID getCurrentSkImage = nullptr;

    jmethodID startMethod = nullptr;

    jmethodID pauseMethod = nullptr;

    jmethodID releaseMethod = nullptr;

    jmethodID deleteSkImageMethod = nullptr;

    jobject javaVideo = nullptr;

    SkImage *skImage = nullptr;

    SkRect dstRect;

    std::unique_ptr<SkPaint> videoPaint;

    long lastSkImagePtr = 0L;

    std::string src;

};

}
