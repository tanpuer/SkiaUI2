//
// Created by banma-3412 on 2024/3/16.
//

#include "ShaderView.h"
#include "SkiaUIContext.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"

ShaderView::ShaderView() {

}

void ShaderView::setShaderSource(const char *data) {
    auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(data));
    if (!effect) {
        ALOGD("set shader source failed %s", error.data())
        return;
    }
    runtimeEffect = effect;
    isDirty = true;
}

void ShaderView::setShaderPath(const char *path) {
    auto assetManager = SkiaUIContext::getInstance()->getAssetManager();
    auto data = assetManager->readFile(path);
    setShaderSource(data);
    isDirty = true;
}

void ShaderView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
}

void ShaderView::draw(SkCanvas *canvas) {
    if (runtimeEffect != nullptr) {
        SkCanvas *skCanvas;
        SkPictureRecorder recorder;
        skCanvas = recorder.beginRecording(width, height);
        ResolutionUniforms uniforms;
        uniforms.width = width;
        uniforms.height = height;
        SkRuntimeShaderBuilder builder(runtimeEffect);
        builder.uniform("iResolution") = uniforms;
        auto time = SkiaUIContext::getInstance()->getCurrentTimeMills();
        builder.uniform("iTime") = (float) time / 1000;
        auto shader = builder.makeShader(nullptr);
        SkPaint skPaint;
        skPaint.setShader(std::move(shader));
        skCanvas->drawIRect({0, 0, width, height}, skPaint);
        auto picture = recorder.finishRecordingAsPicture();
        canvas->save();
        canvas->translate(left, top);
        canvas->drawPicture(picture);
        canvas->restore();
    }
}
