#pragma once

#include "View.h"
#include "memory"

namespace HYSkiaUI {

class CanvasTest : public View {

public:

    CanvasTest();

    ~CanvasTest();

    void draw(SkCanvas *canvas) override;

    void setCircleSize(int size);

private:

    std::unique_ptr<SkPaint> paint;
    std::unique_ptr<SkPaint> pathPaint;

    int size = 0;

    long testIndex = 0;
};

}
