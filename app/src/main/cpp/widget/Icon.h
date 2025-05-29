#pragma once

#include "View.h"

namespace HYSkiaUI {

class Icon : public View {

public:

    Icon();

    ~Icon() override;

    void measure() override;

    void draw(SkCanvas *canvas) override;

    void setIcon(int32_t text);

    int32_t getIcon();

    void setIconSize(int size);

    int getIconSize();

    void setIconColor(SkColor color);

    void setIconColor(const char *color);

    const char *getIconColor();

    void setAlpha(float alpha) override;

private:

    int32_t text = 0;

    SkGlyphID glyphId = 0;

    std::unique_ptr<SkFont> font = nullptr;

    int size = 100;

    std::unique_ptr<SkPaint> iconPaint = nullptr;

    float x = 0.0;

    float y = 0.0;

    const char *iconColor = nullptr;

};

}
