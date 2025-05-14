#include "BoxDecoration.h"

namespace HYSkiaUI {

BoxDecoration::BoxDecoration() {
    shadowPaint = std::make_unique<SkPaint>();
    shadowPaint->setAntiAlias(true);
    bgColorPaint = std::make_unique<SkPaint>();
    bgColorPaint->setAntiAlias(true);
    bgImagePaint = std::make_unique<SkPaint>();
    bgImagePaint->setAntiAlias(true);
    radiusPaint = std::make_unique<SkPaint>();
    radiusPaint->setAntiAlias(true);
}

BoxDecoration::~BoxDecoration() {

}

void BoxDecoration::setLefTopRadius(float radius) {
    leftTopRadius = radius;
}

void BoxDecoration::setLeftBottomRadius(float radius) {
    leftBottomRadius = radius;
}

void BoxDecoration::setRightTopRadius(float radius) {
    rightTopRadius = radius;
}

void BoxDecoration::setRightBottomRadius(float radius) {
    rightBottomRadius = radius;
}

void BoxDecoration::setBgBledMode(SkBlendMode mode) {

}

void BoxDecoration::setBgColor(SkColor color) {
    bgColor = color;
}

void BoxDecoration::setBgImage(const char *image) {

}

void BoxDecoration::setShadow(float sigma) {
    shadowSigma = sigma;
}

void BoxDecoration::draw(SkCanvas *canvas, SkRect &rect) {

}

}
