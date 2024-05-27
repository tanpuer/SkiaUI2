#pragma once

#include "View.h"
#include "string"
#include "core/SkImage.h"
#include "View.h"

class VideoView : public View {

public:

    VideoView();

    virtual ~VideoView();

    void setSource(const char *path);

    void start();

    void pause();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

private:

    jclass javaVideoClass = nullptr;

    jmethodID javaVideoConstructor = nullptr;

    jmethodID getCurrentSkImage = nullptr;

    jobject javaVideo = nullptr;

    sk_sp<SkImage> skImage = nullptr;

    SkRect dstRect;

    std::unique_ptr<SkPaint> videoPaint;

};
