//
// Created by banma-3412 on 2024/3/16.
//

#ifndef SKIAUI_SHADERVIEW_H
#define SKIAUI_SHADERVIEW_H


#include "View.h"
#include "effects/SkRuntimeEffect.h"
#include "vector"
#include "core/SkImage.h"

class ShaderView : public View {

public:

    ShaderView();

    virtual ~ShaderView();

    virtual void setShaderSource(const char *data, std::vector<std::string> images = {});

    virtual void setShaderPath(const char *path, std::vector<std::string> images = {});

    virtual void draw(SkCanvas *canvas) override;

private:

    sk_sp<SkRuntimeEffect> runtimeEffect;

    std::vector<sk_sp<SkShader>> skShaders;

    std::vector<std::string> imageNames;

};


#endif //SKIAUI_SHADERVIEW_H
