#include "Icon.h"
#include "core/SkFont.h"

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
    isDirty = true;
}

void Icon::setIconSize(int size) {
    this->size = size;
    isDirty = true;
}

void Icon::setIconColor(SkColor color) {
    iconPaint->setColor(color);
}
