#pragma once

#include "BaseSurfaceTextureView.h"

namespace HYSkiaUI {

class ComposeTextureView: public BaseSurfaceTextureView {

public:

    ComposeTextureView();

    ~ComposeTextureView();

    void setContext(std::shared_ptr<SkiaUIContext> &context) override;

    const char * getJavaClassPath() override;

    virtual void checkJavaViewRef(jobject instance);

};

}