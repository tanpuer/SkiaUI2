#pragma once

#include "View.h"

namespace HYSkiaUI {

class FilamentView : public View {

public:

    FilamentView();

    ~FilamentView();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void onShow() override;

    void onHide() override;

    bool onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    bool onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

private:

    SkRect dstRect;

    bool inited = false;

    jclass javaFilamentClass = nullptr;

    jmethodID javaFilamentConstructor = nullptr;

    jobject javaFilament = nullptr;

    jmethodID getSkImageMethodId = nullptr;

    long lastSkImagePtr = 0L;

    SkImage *skImage = nullptr;

    std::unique_ptr<SkPaint> platformPaint;

    jmethodID releaseMethod = nullptr;

    jmethodID showMethod = nullptr;

    jmethodID hideMethod = nullptr;

    jmethodID sendTouchEventMethodId = nullptr;

};

}
