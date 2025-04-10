#pragma once

#include "View.h"

namespace HYSkiaUI {

/**
 * third-party engine render to surfaceTexture and than transformed to SkImage.
 * such as: ExoPlayer, Filament...
 */
class BaseSurfaceTextureView : public View {

public:

    BaseSurfaceTextureView();

    ~BaseSurfaceTextureView();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void onShow() override;

    void onHide() override;

    virtual const char *getJavaClassPath() = 0;

    virtual void initJNI() {}

    virtual void drawOneFrame() {}

    bool onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    bool onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    void onSizeChange(int width, int height) override;

protected:

    SkRect dstRect;

    bool inited = false;

    jclass javaClass = nullptr;

    jmethodID javaConstructor = nullptr;

    jobject javaInstance = nullptr;

    jmethodID getSkImageMethodId = nullptr;

    long lastSkImagePtr = 0L;

    SkImage *skImage = nullptr;

    std::unique_ptr<SkPaint> platformPaint;

    jmethodID releaseMethod = nullptr;

    jmethodID showMethod = nullptr;

    jmethodID hideMethod = nullptr;

    jmethodID sendTouchEventMethodId = nullptr;

    jmethodID onSizeChangeMethodId = nullptr;

};

}
