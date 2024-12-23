#include "WebView.h"

namespace HYSkiaUI {

WebView::WebView() {
    webPaint = std::make_unique<SkPaint>();
    webPaint->setAntiAlias(true);
}

WebView::~WebView() {
    auto jniEnv = getContext()->getJniEnv();
    if (javaWebView != nullptr) {
        jniEnv->CallVoidMethod(javaWebView, releaseMethodId);
        jniEnv->DeleteGlobalRef(javaWebView);
    }
}

void WebView::loadUrl(const char *url) {
    this->url = url;
    if (javaWebView != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        auto jStr = jniEnv->NewStringUTF(url);
        jniEnv->CallVoidMethod(javaWebView, loadUrlMethodId, jStr);
    }
}

void WebView::draw(SkCanvas *canvas) {
    auto jniEnv = getContext()->getJniEnv();
    auto skImagePtr = jniEnv->CallLongMethod(javaWebView, getSkImageMethodId);
    if (skImagePtr != lastSkImagePtr) {
        if (skImagePtr != 0L) {
            auto image = reinterpret_cast<SkImage *>(skImagePtr);
            skImage = SkSafeRef(image);
        } else {
            skImage = nullptr;
        }
        lastSkImagePtr = skImagePtr;
    }
    canvas->drawImageRect(skImage, dstRect, SkSamplingOptions(), webPaint.get());
    markDirty();
}

void WebView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    dstRect.setLTRB(static_cast<float >(l), static_cast<float >(t), static_cast<float >(r),
                    static_cast<float >(b));
    if (javaWebView == nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        auto javaWebViewPlugin = jniEnv->FindClass(
                "com/temple/skiaui/platform/PlatformWebViewPlugin");
        getSkImageMethodId = jniEnv->GetMethodID(javaWebViewPlugin, "getSkImage", "()J");
        releaseMethodId = jniEnv->GetMethodID(javaWebViewPlugin, "release", "()V");
        loadUrlMethodId = jniEnv->GetMethodID(javaWebViewPlugin, "loadUrl",
                                              "(Ljava/lang/String;)V");
        sendTouchEventMethodId = jniEnv->GetMethodID(javaWebViewPlugin, "sendTouchEvent", "(IFF)V");
        auto javaConstructor = jniEnv->GetMethodID(javaWebViewPlugin, "<init>",
                                                   "(Lcom/temple/skiaui/HYSkiaEngine;II)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        javaWebView = jniEnv->NewGlobalRef(jniEnv->NewObject(javaWebViewPlugin, javaConstructor,
                                                             javaSkiaEngine, this->width,
                                                             this->height));
        if (!this->url.empty()) {
            auto jStr = jniEnv->NewStringUTF(this->url.c_str());
            jniEnv->CallVoidMethod(javaWebView, loadUrlMethodId, jStr);
        }
    }
}

bool WebView::onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    if (javaWebView != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        jniEnv->CallVoidMethod(javaWebView, sendTouchEventMethodId,
                               static_cast<int>(touchEvent->action),
                               touchEvent->x - left, touchEvent->y - top);
    }
    return true;
}

bool WebView::onInterceptTouchEvent(HYSkiaUI::TouchEvent *touchEvent) {
    return true;
}

}
