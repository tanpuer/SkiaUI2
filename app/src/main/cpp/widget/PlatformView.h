#pragma once

#include "View.h"

namespace HYSkiaUI {

class PlatformView : public View {

public:

    PlatformView();

    ~PlatformView();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    bool onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    bool onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    virtual const char *getJavaPlatformViewName() = 0;

    virtual void onJavaViewCreated() {}

protected:

    SkRect dstRect;

    long lastSkImagePtr = 0L;

    SkImage *skImage = nullptr;

    std::unique_ptr<SkPaint> platformPaint;

    jobject javaView = nullptr;

    jmethodID getSkImageMethodId = nullptr;

    jmethodID sendTouchEventMethodId = nullptr;

    jmethodID deleteSkImageMethodId = nullptr;

    jmethodID releaseMethodId = nullptr;

};

}