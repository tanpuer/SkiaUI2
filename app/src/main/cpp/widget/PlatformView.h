#pragma once

#include "View.h"

namespace HYSkiaUI {

/**
 * Android Native Views render to surfaceTexture and than transformed to SkImage.
 * such as: EditText, WebView...
 */
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

    void setContext(std::shared_ptr<SkiaUIContext> &context) override;

    void onSizeChange(int width, int height) override;

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

    jmethodID onSizeChangeMethodId = nullptr;

    bool firstResize = false;

};

}
