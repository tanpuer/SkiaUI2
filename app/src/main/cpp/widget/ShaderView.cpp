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
    skShaders.clear();
    imageResolutions.clear();
    uniformVector.clear();
    runtimeEffect = nullptr;
    auto createEffect = [this, data]() {
        auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(data));
        if (!effect) {
            ALOGD("set shader source failed %s", error.data())
            return;
        }
        runtimeEffect = effect;
        markDirty();
    };
    auto size = images.size();
    if (size == 0) {
        createEffect();
        return;
    }
    for (int i = 0; i < images.size(); ++i) {
        auto image = images[i];
        context->resourcesLoader->decodeImage(
                image,
                [this, i](const std::vector<sk_sp<SkImage>> &images,
                          sk_sp<SkAnimatedImage> animatedImage) {
                    auto skImage = animatedImage->getCurrentFrame();
                    auto shader = skImage->makeShader(SkSamplingOptions());
                    skShaders["iChannel" + std::to_string(i)] = std::move(shader);
                    ResolutionUniforms resolutionUniforms;
                    resolutionUniforms.width = skImage->width();
                    resolutionUniforms.height = skImage->height();
                    imageResolutions["iChannel" + std::to_string(i) +
                                     "Resolution"] = resolutionUniforms;
                });
        size--;
        if (size == 0) {
            createEffect();
        }
    }
}

void ShaderView::setShaderPath(const char *path, std::vector<std::string> images) {
    MeasureTime measureTime("setShaderPath");
    context->resourcesLoader->readFile(path, [this, images](const char *data) {
        setShaderSource(data, images);
        markDirty();
    });
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
        auto time = getContext()->getCurrentTimeMills();
        builder.uniform("iTime") = (float) time / 1000;
        builder.uniform("shaderTouchX") = shaderTouchX;
        for (const auto &item: uniformVector) {
            builder.uniform(item.first) = item.second;
        }
        for (auto &pair: skShaders) {
            builder.child(pair.first) = pair.second;
        }
        for (auto &pair: imageResolutions) {
            builder.uniform(pair.first) = pair.second;
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

void ShaderView::setPictures(std::vector<sk_sp<SkPicture>> otherPictures) {
    skShaders.clear();
    for (int i = 0; i < otherPictures.size(); ++i) {
        auto shader = otherPictures[i]->makeShader(SkTileMode::kClamp, SkTileMode::kClamp,
                                                   SkFilterMode::kLinear);
        skShaders["iChannel" + std::to_string(i)] = std::move(shader);
    }
    markDirty();
}

void ShaderView::setCustomUniforms(std::string key, float value) {
    uniformVector[key] = value;
}

const char *ShaderView::name() {
    return "ShaderView";
}

bool ShaderView::onTouchEvent(TouchEvent *touchEvent) {
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN:
        case TouchEvent::ACTION_MOVE: {
            shaderTouchX = touchEvent->x - left;
            break;
        }
        case TouchEvent::ACTION_UP:
        case TouchEvent::ACTION_CANCEL: {
            shaderTouchX = -1.0f;
            break;
        }
    }
    return View::onTouchEvent(touchEvent);
}

bool ShaderView::onInterceptTouchEvent(TouchEvent *touchEvent) {
    switch (touchEvent->action) {
        case TouchEvent::ACTION_DOWN: {
            lastScrollX = touchEvent->x;
            lastScrollY = touchEvent->y;
            break;
        }
        case TouchEvent::ACTION_MOVE: {
            auto diffX = abs(touchEvent->x - lastScrollX);
            auto diffY = abs(touchEvent->y - lastScrollY);
            shaderTouchX = touchEvent->x - left;
            return diffX > diffY;
        }
        default:
            break;
    }
    return true;
}
