#pragma once

#include "View.h"

namespace HYSkiaUI {

class ComposeCanvas : public View {

public:

    ComposeCanvas();

    ~ComposeCanvas();

    void draw(SkCanvas *canvas) override;

    void setContext(std::shared_ptr<SkiaUIContext> &context) override;

    SkCanvas* getCanvas();

private:

    SkCanvas *canvas = nullptr;

    void callComposeDraw(SkCanvas *canvas);

    jmethodID onDrawMethodId = nullptr;

};

}
