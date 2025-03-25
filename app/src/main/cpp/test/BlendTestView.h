#pragma once

#include "View.h"

namespace HYSkiaUI {

class BlendTestView : public View {

public:

    BlendTestView();

    ~BlendTestView();

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void setBlendMode(SkBlendMode mode);

private:

    std::unique_ptr<SkPaint> testPaint;

    std::unique_ptr<SkPaint> discP;

    int size = 200;

    SkBlendMode mode;

    SkRect testRect;

};

}
