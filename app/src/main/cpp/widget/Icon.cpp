#include "Icon.h"
#include "core/SkFont.h"
#include "color_util.h"

Icon::Icon() : View() {
    iconPaint = std::make_unique<SkPaint>();
    iconPaint->setAntiAlias(true);
    iconPaint->setColor(SK_ColorGREEN);
}

Icon::~Icon() {

}

void Icon::measure() {
    if (isDirty) {
        auto typeface = getContext()->getIconFontTypeFace();
        font = std::make_unique<SkFont>(typeface, size);
        this->glyphId = typeface->unicharToGlyph(text);
        SkPath path;
        font->getPath(glyphId, &path);
        SkRect bounds = path.getBounds();
        width = bounds.width();
        height = bounds.height();
        x = bounds.x();
        y = bounds.y();
        setMeasuredDimension(width, height);
        clearDirty();
    }
}

void Icon::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (text == 0) {
        return;
    }
    auto position = SkPoint::Make(left, top);
    canvas->drawGlyphs(1, &glyphId, &position, SkPoint::Make(-x, -y), *font, *iconPaint);
}

void Icon::setIcon(int32_t text) {
    this->text = text;
    markDirty();
}

void Icon::setIconSize(int size) {
    this->size = size;
    markDirty();
}

void Icon::setIconColor(SkColor color) {
    iconPaint->setColor(color);
}

void Icon::setAlpha(float alpha) {
    View::setAlpha(alpha);
    iconPaint->setAlphaf(alpha);
}

int32_t Icon::getIcon() {
    return text;
}

int Icon::getIconSize() {
    return size;
}

const char *Icon::getIconColor() {
    return iconColor;
}

void Icon::setIconColor(const char *color) {
    iconColor = color;
    int r, g, b, a;
    hexToRGBA(color, r, g, b, a);
    setIconColor(SkColorSetARGB(a, r, g, b));
}

