#pragma once

#include "PlatformView.h"

namespace HYSkiaUI {

class WebView : public PlatformView {

public:

    WebView();

    ~WebView() override;

    void loadUrl(const char *url);

    void setProgress(int progress);

    void setProgressCallback(std::function<void(int)> &&callback);

    const char *getJavaPlatformViewName() override;

    void onJavaViewCreated() override;

    const char *name() override;

private:

    jmethodID loadUrlMethodId = nullptr;

    std::string url;

    std::function<void(int)> progressCallback = nullptr;
};

}
