#include "FilamentView.h"

namespace HYSkiaUI {

FilamentView::FilamentView() {
    platformPaint = std::make_unique<SkPaint>();
    platformPaint->setAntiAlias(true);
}

FilamentView::~FilamentView() {
    if (javaFilament != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaFilament, releaseMethod);
        jniEnv->DeleteGlobalRef(javaFilament);
        javaFilament = nullptr;
    }
}

void FilamentView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
    if (!inited) {
        inited = true;
        auto jniEnv = context->getJniEnv();
        javaFilamentClass = jniEnv->FindClass(
                "com/temple/skiaui/platform/filament/PlatformFilamentViewPlugin");
        javaFilamentConstructor = jniEnv->GetMethodID(javaFilamentClass, "<init>",
                                                      "(Lcom/temple/skiaui/HYSkiaEngine;IIJ)V");
        getSkImageMethodId = jniEnv->GetMethodID(javaFilamentClass, "getSkImage", "()J");
        showMethod = jniEnv->GetMethodID(javaFilamentClass, "onShow", "()V");
        hideMethod = jniEnv->GetMethodID(javaFilamentClass, "onHide", "()V");
        releaseMethod = jniEnv->GetMethodID(javaFilamentClass, "release", "()V");
        sendTouchEventMethodId = jniEnv->GetMethodID(javaFilamentClass, "sendTouchEvent", "(IFF)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        javaFilament = jniEnv->NewGlobalRef(
                jniEnv->NewObject(javaFilamentClass, javaFilamentConstructor,
                                  javaSkiaEngine, width, height, reinterpret_cast<long>(this)));
    }
}

void FilamentView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaFilament, getSkImageMethodId);
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
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), platformPaint.get());
}

void FilamentView::onShow() {
    View::onShow();
    if (javaFilament != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaFilament, showMethod);
    }
}

void FilamentView::onHide() {
    View::onHide();
    if (javaFilament != nullptr) {
        getContext()->getJniEnv()->CallVoidMethod(javaFilament, hideMethod);
    }
}

bool FilamentView::onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    return false;
}

bool FilamentView::onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    if (javaFilament != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaFilament, sendTouchEventMethodId,
                               static_cast<int>(touchEvent->action),
                               touchEvent->x - left, touchEvent->y - top);
    }
    return true;
}

}
