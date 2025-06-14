#include "WebView.h"

namespace HYSkiaUI {

WebView::WebView() {
}

WebView::~WebView() {
}

void WebView::loadUrl(const char *url) {
    this->url = url;
    if (javaView != nullptr) {
        auto jniEnv = getContext()->getJniEnv();
        auto jStr = jniEnv->NewStringUTF(url);
        jniEnv->CallVoidMethod(javaView, loadUrlMethodId, jStr);
        jniEnv->DeleteLocalRef(jStr);
    }
}

const char *WebView::getJavaPlatformViewName() {
    return "com/temple/skiaui/platform/webview/PlatformWebViewPlugin";
}

void WebView::setProgress(int progress) {
    if (progressCallback != nullptr) {
        progressCallback(progress);
    }
}

void WebView::setProgressCallback(std::function<void(int)> &&callback) {
    progressCallback = std::move(callback);
}

void WebView::onJavaViewCreated() {
    PlatformView::onJavaViewCreated();
    auto jniEnv = getContext()->getJniEnv();
    auto javaWebViewPlugin = jniEnv->FindClass(getJavaPlatformViewName());
    loadUrlMethodId = jniEnv->GetMethodID(javaWebViewPlugin, "loadUrl",
                                          "(Ljava/lang/String;)V");
}

const char *WebView::name() {
    return "WebView";
}

}
