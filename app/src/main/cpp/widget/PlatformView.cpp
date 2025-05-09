#include "PlatformView.h"

namespace HYSkiaUI {

PlatformView::PlatformView() {
    platformPaint = std::make_unique<SkPaint>();
    platformPaint->setAntiAlias(true);
}

PlatformView::~PlatformView() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, releaseMethodId);
    if (javaView != nullptr) {
        jniEnv->DeleteGlobalRef(javaView);
    }
}

void PlatformView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
    if (!firstResize) {
        onSizeChange(width, height);
    }
}

void PlatformView::onSizeChange(int width, int height) {
    if (width > 0 && height > 0) {
        firstResize = true;
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaView, onSizeChangeMethodId, width, height);
    }
}

void PlatformView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaView, getSkImageMethodId);
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
    }
    canvas->save();
    canvas->setMatrix(viewMatrix);
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), platformPaint.get());
    canvas->restore();
}

bool PlatformView::onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    return true;
}

bool PlatformView::onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    if (javaView != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaView, sendTouchEventMethodId,
                               static_cast<int>(touchEvent->action),
                               touchEvent->x - left, touchEvent->y - top);
    }
    return true;
}

void PlatformView::setContext(std::shared_ptr<SkiaUIContext> &context) {
    View::setContext(context);
    auto jniEnv = getContext()->getJniEnv();
    auto javaPluginClazz = jniEnv->FindClass(getJavaPlatformViewName());
    getSkImageMethodId = jniEnv->GetMethodID(javaPluginClazz, "getSkImage", "()J");
    sendTouchEventMethodId = jniEnv->GetMethodID(javaPluginClazz, "sendTouchEvent", "(IFF)V");
    deleteSkImageMethodId = jniEnv->GetMethodID(javaPluginClazz, "deleteSkImage", "(J)V");
    releaseMethodId = jniEnv->GetMethodID(javaPluginClazz, "release", "()V");
    onSizeChangeMethodId = jniEnv->GetMethodID(javaPluginClazz, "onSizeChange", "(II)V");
    setBackgroundColorMethodId = jniEnv->GetMethodID(javaPluginClazz, "setBackgroundColor", "(I)V");
    auto javaConstructor = jniEnv->GetMethodID(javaPluginClazz, "<init>",
                                               "(Lcom/temple/skiaui/HYSkiaEngine;J)V");
    auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
    javaView = jniEnv->NewGlobalRef(
            jniEnv->NewObject(javaPluginClazz, javaConstructor, javaSkiaEngine,
                              reinterpret_cast<long>(this)));
    onJavaViewCreated();
}

void PlatformView::setBackgroundColor(SkColor color) {
    View::setBackgroundColor(color);
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(javaView, setBackgroundColorMethodId, color);
}

}
