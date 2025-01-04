#pragma once

#include "View.h"
#include "effects/SkRuntimeEffect.h"

namespace HYSkiaUI {

enum class YUVFormat {
    YUV420,
    NV12,
};

class CameraView : public View {

public:

    CameraView();

    ~CameraView();

    const char *name() override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void setContext(std::shared_ptr<SkiaUIContext> context) override;

    void onShow() override;

    void onHide() override;

private:

    void initShader(YUVFormat format);

    jclass javaCameraClass = nullptr;

    jmethodID javaCameraConstructor = nullptr;

    jmethodID releaseMethod = nullptr;

    jmethodID showMethod = nullptr;

    jmethodID hideMethod = nullptr;

    jmethodID getYUVDataMethod = nullptr;

    jobject javaCamera = nullptr;

    std::unique_ptr<SkPaint> cameraPaint;

    sk_sp<SkRuntimeEffect> runtimeEffect = nullptr;

    bool inited = false;

    YUVFormat yuvFormat = YUVFormat::YUV420;
};

}
