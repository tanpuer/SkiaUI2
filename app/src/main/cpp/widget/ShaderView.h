#pragma once

#include "View.h"
#include "effects/SkRuntimeEffect.h"
#include "vector"
#include "core/SkImage.h"
#include "unordered_map"
#include "AndroidBitmap.h"

namespace HYSkiaUI {

class ShaderView : public View {

public:

    ShaderView();

    virtual ~ShaderView();

    const char *name() override;

    virtual void setShaderSource(const char *data, std::vector<std::string> images = {});

    virtual void setShaderPath(const char *path, std::vector<std::string> images = {});

    virtual void draw(SkCanvas *canvas) override;

    virtual void setCustomUniforms(std::string key, float value);

    virtual void setPictures(std::vector<sk_sp<SkPicture>> otherPictures);

    bool onTouchEvent(TouchEvent *touchEvent) override;

    bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

private:

    sk_sp<SkRuntimeEffect> runtimeEffect;

    std::unordered_map<std::string, sk_sp<SkShader>> skShaders;

    std::unordered_map<std::string, ResolutionUniforms> imageResolutions;

    std::unordered_map<std::string, float> uniformVector;

    float shaderTouchX = -1.0f;
    float lastScrollX = 0.0f;
    float lastScrollY = 0.0f;

    std::vector<std::unique_ptr<AndroidBitmap>> androidBitmaps;

};

}
