#include "BaseSurfaceTextureView.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"

namespace HYSkiaUI {

BaseSurfaceTextureView::BaseSurfaceTextureView() {
}

BaseSurfaceTextureView::~BaseSurfaceTextureView() {
    if (javaInstance != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaInstance, releaseMethod);
        jniEnv->DeleteGlobalRef(javaInstance);
        javaInstance = nullptr;
    }
}

void BaseSurfaceTextureView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
    if (!inited) {
        inited = true;
        auto jniEnv = context->getJniEnv();
        javaClass = jniEnv->FindClass(getJavaClassPath());
        javaConstructor = jniEnv->GetMethodID(javaClass, "<init>",
                                              "(Lcom/temple/skiaui/HYSkiaEngine;IIJ)V");
        getSkImageMethodId = jniEnv->GetMethodID(javaClass, "getSkImage", "()J");
        showMethod = jniEnv->GetMethodID(javaClass, "onShow", "()V");
        hideMethod = jniEnv->GetMethodID(javaClass, "onHide", "()V");
        releaseMethod = jniEnv->GetMethodID(javaClass, "release", "()V");
        sendTouchEventMethodId = jniEnv->GetMethodID(javaClass, "sendTouchEvent", "(IFF)V");
        onSizeChangeMethodId = jniEnv->GetMethodID(javaClass, "onSizeChange", "(II)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        javaInstance = jniEnv->NewGlobalRef(
                jniEnv->NewObject(javaClass, javaConstructor,
                                  javaSkiaEngine, width, height, reinterpret_cast<long>(this)));
        initJNI();
    }
}

void BaseSurfaceTextureView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaInstance, getSkImageMethodId);
    if (skImagePtr != lastSkImagePtr) {
        if (skImagePtr != 0L) {
            auto image = reinterpret_cast<SkImage *>(skImagePtr);
            SkSafeUnref(skImage);
            skImage = SkSafeRef(image);
        } else {
            SkSafeUnref(skImage);
            skImage = nullptr;
        }
        lastSkImagePtr = skImagePtr;
        drawOneFrame();
    }
    if (runtimeEffect != nullptr && skImage != nullptr) {
        drawShader(canvas);
        return;
    }
    canvas->save();
    canvas->setMatrix(viewMatrix);
    if (cornerRadius > 0) {
        rRect.setRectXY(dstRect, cornerRadius, cornerRadius);
        canvas->clipRRect(rRect);
    }
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), paint.get());
    canvas->restore();
}

void BaseSurfaceTextureView::onShow() {
    View::onShow();
    if (javaInstance != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaInstance, showMethod);
    }
}

void BaseSurfaceTextureView::onHide() {
    View::onHide();
    if (javaInstance != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaInstance, hideMethod);
    }
}

bool BaseSurfaceTextureView::onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    return false;
}

bool BaseSurfaceTextureView::onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    if (javaInstance != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaInstance, sendTouchEventMethodId,
                               static_cast<int>(touchEvent->action),
                               touchEvent->x - left, touchEvent->y - top);
    }
    return true;
}

void BaseSurfaceTextureView::onSizeChange(int width, int height) {
    if (javaInstance != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaInstance, onSizeChangeMethodId, width, height);
    }
}

void BaseSurfaceTextureView::setShaderPath(const char *path) {
    if (shaderPath == path) {
        return;
    }
    context->resourcesLoader->readFile(path, [this](const char *data) {
        setShaderCode(data);
    });
}

void BaseSurfaceTextureView::drawShader(SkCanvas *canvas) {
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
    auto skShader = skImage->makeShader(SkSamplingOptions());
    builder.child("iChannel0") = std::move(skShader);
    auto shader = builder.makeShader(nullptr);
    SkPaint skPaint;
    skPaint.setShader(std::move(shader));
    skCanvas->drawIRect({0, 0, width, height}, skPaint);
    auto picture = recorder.finishRecordingAsPicture();
    canvas->setMatrix(viewMatrix);
    canvas->save();
    canvas->translate(left, top);
    canvas->drawPicture(picture);
    canvas->restore();
}

void BaseSurfaceTextureView::setShaderCode(const char *code) {
    auto [effect, error] = SkRuntimeEffect::MakeForShader(SkString(code));
    if (!effect) {
        ALOGD("set shader source failed %s", error.data())
        return;
    }
    runtimeEffect = effect;
    markDirty();
}

}
