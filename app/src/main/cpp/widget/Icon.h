#pragma once

#include "View.h"


class Icon : public View {

public:

    Icon();

    ~Icon();

    void measure() override;

    void draw(SkCanvas *canvas) override;

    void setIcon(int32_t text);

    void setIconSize(int size);

    void setIconColor(SkColor color);

    void setAlpha(float alpha) override;

private:

    int32_t text = 0;

    SkGlyphID glyphId = 0;

    std::unique_ptr<SkFont> font = nullptr;

    int size = 100;

    std::unique_ptr<SkPaint> iconPaint = nullptr;

    float x = 0.0;

    float y = 0.0;

};
