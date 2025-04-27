#include "WebViewPage.h"
#include "FlexboxLayout.h"
#include "WebView.h"
#include "EditText.h"
#include "TextView.h"

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
        auto textView = new TextView();
        textView->setContext(this->context);
        textView->setText(SkString("This is native EditText && WebView"));
        textView->setTextColor(SK_ColorBLACK);
        textView->setTextSize(50);
        textView->setBackgroundColor(SK_ColorTRANSPARENT);
        textView->setStyle(SkPaint::kStroke_Style);
        flexboxLayout->addView(textView);
    }

    {
        auto editText = new EditText();
        editText->setContext(this->context);
        editText->setWidth(width);
        editText->setHeight(200);
        flexboxLayout->addView(editText);
    }

    {
        progressView = new WebViewProgressView();
        progressView->setContext(this->context);
        progressView->setWidth(width);
        progressView->setHeight(16);
        flexboxLayout->addView(progressView);
    }

    {
        auto webView = new WebView();
        webView->setContext(this->context);
        webView->setWidth(width);
        webView->setFlex(1);
        webView->loadUrl("https://m.bilibili.com/");
        webView->setStyle(SkPaint::kStroke_Style);
        webView->setBackgroundColor(SK_ColorTRANSPARENT);
        webView->setMargin({0, 200, 0, 0});
        webView->setStrokeWidth(0);
        flexboxLayout->addView(webView);
        webView->setProgressCallback([this](int progress) {
            if (progressView != nullptr) {
                progressView->setProgress(progress);
            }
        });
    }
}

}