//
// Created by banma-3412 on 2024/3/16.
//

#include "ShaderView.h"
#include "SkiaUIContext.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "codec/SkAndroidCodec.h"
#include "android/SkAnimatedImage.h"

ShaderView::ShaderView() {

}

ShaderView::~ShaderView() {

}

void ShaderView::setShaderSource(const char *data, std::vector<std::string> images) {
    auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(data));
    if (!effect) {
        ALOGD("set shader source failed %s", error.data())
        return;
    }
    runtimeEffect = effect;
    auto assetManager = SkiaUIContext::getInstance()->getAssetManager();
    for (int i = 0; i < images.size(); ++i) {
        auto image = images[i];
        auto imageData = assetManager->readImage(image.c_str());
        auto length = imageData->length;
        auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
        auto androidCodec = SkAndroidCodec::MakeFromData(skData);
        auto skAnimatedImage = SkAnimatedImage::Make(std::move(androidCodec));
        auto skImage = skAnimatedImage->getCurrentFrame();
        auto shader = skImage->makeShader(SkTileMode::kClamp, SkTileMode::kClamp,
                                          SkFilterMode::kLinear);
        skShaders.push_back(std::move(shader));
        imageNames.push_back("iChannel" + std::to_string(i));
    }
    isDirty = true;
}

void ShaderView::setShaderPath(const char *path, std::vector<std::string> images) {
    auto assetManager = SkiaUIContext::getInstance()->getAssetManager();
    auto data = assetManager->readFile(path);
    setShaderSource(data, images);
    isDirty = true;
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
        for (int i = 0; i < skShaders.size(); ++i) {
            builder.child(imageNames[i]) = skShaders[i];
        }
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
