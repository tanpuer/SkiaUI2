//
// Created by banma-3412 on 2024/3/16.
//

#ifndef SKIAUI_SHADERVIEW_H
#define SKIAUI_SHADERVIEW_H


#include "View.h"
#include "effects/SkRuntimeEffect.h"

class ShaderView : public View {

public:

    ShaderView();

    virtual void setShaderSource(const char *data);

    virtual void setShaderPath(const char *path);

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

private:

    sk_sp<SkRuntimeEffect> runtimeEffect;

};


#endif //SKIAUI_SHADERVIEW_H
