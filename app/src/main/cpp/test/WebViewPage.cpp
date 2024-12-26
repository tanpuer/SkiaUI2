#include "WebViewPage.h"
#include "FlexboxLayout.h"
#include "WebView.h"

namespace HYSkiaUI {

void WebViewPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
    setContext(context);
    setWidth(width);
    setHeight(height);
    setFlexWrap(YGWrapWrap);
    setFlexDirection(YGFlexDirectionColumn);
    setJustifyContent(YGJustifyCenter);
    setAlignItems(YGAlignCenter);
    setAlignContent(YGAlignCenter);
    setStyle(SkPaint::kFill_Style);
    setBackgroundColor(SK_ColorTRANSPARENT);
    initChildren(this, width, height);
}

void WebViewPage::initChildren(HYSkiaUI::ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    flexboxLayout->setContext(this->context);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setFlex(1);
    this->addView(flexboxLayout);

    {
        auto webView = new WebView();
        webView->setContext(this->context);
        webView->setWidth(width);
        webView->setHeight(height);
        webView->setPositionType(YGPositionTypeAbsolute);
        webView->loadUrl("https://m.bilibili.com/");
        webView->setStyle(SkPaint::kStroke_Style);
        webView->setBackgroundColor(SK_ColorTRANSPARENT);
        webView->setStrokeWidth(0);
        flexboxLayout->addView(webView);
        webView->setProgressCallback([this](int progress) {
            if (progressView != nullptr) {
                progressView->setProgress(progress);
            }
        });
    }

    {
        progressView = new WebViewProgressView();
        progressView->setContext(this->context);
        progressView->setWidth(width);
        progressView->setHeight(16);
        progressView->setPositionType(YGPositionTypeAbsolute);
        flexboxLayout->addView(progressView);
    }
}

}