#pragma once

#include "View.h"

namespace HYSkiaUI {

class WebView : public View {

public:

    WebView();

    ~WebView();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void loadUrl(const char *url);

    bool onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    bool onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    void setProgress(int progress);

    void setProgressCallback(std::function<void(int)> &&callback);

private:

    SkRect dstRect;

    long lastSkImagePtr = 0L;

    SkImage *skImage = nullptr;

    std::unique_ptr<SkPaint> webPaint;

    jmethodID getSkImageMethodId = nullptr;

    jmethodID releaseMethodId = nullptr;

    jmethodID loadUrlMethodId = nullptr;

    jmethodID sendTouchEventMethodId = nullptr;

    jobject javaWebView = nullptr;

    std::string url;

    std::function<void(int)> progressCallback = nullptr;
};

}
