#include "SVGView.h"

#include <utility>
#include "core/SkStream.h"
#include "svg/include/SkSVGDOM.h"
#include "SkiaUIContext.h"

namespace HYSkiaUI {

SVGView::SVGView() {
    paint->setColor(SK_ColorWHITE);
}

SVGView::~SVGView() {

}

void SVGView::setSource(const char *path) {
    MeasureTime measureTime("SVG setSource");
    src = path;
    context->resourcesLoader->decodeSVG(src, [this](sk_sp<SkSVGDOM> svg) {
        this->skSVGDom = std::move(svg);
    });
}

void SVGView::setText(const std::string &text) {
    auto stream = SkMemoryStream::MakeDirect(text.c_str(), text.size());
    skSVGDom = SkSVGDOM::Builder().make(*stream);
}

void SVGView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (skSVGDom != nullptr) {
        canvas->save();
        canvas->setMatrix(svgMatrix);
        skSVGDom->render(canvas);
        canvas->restore();
    }
}

void SVGView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (skSVGDom != nullptr && (containerWidth != width || containerHeight != height)) {
        containerWidth = width;
        containerHeight = height;
        skSVGDom->setContainerSize(SkSize::Make(static_cast<float >(containerWidth),
                                                static_cast<float >(containerHeight)));
    }
    svgMatrix.setIdentity();
    svgMatrix.preTranslate(static_cast<float >(left + x), static_cast<float >(top + y));
    svgMatrix.preRotate(rotateZ, static_cast<float >(width) / 2, static_cast<float >(height) / 2);
}

void SVGView::setXY(int x, int y) {
    this->x = x;
    this->y = y;
    markDirty();
}

const char *SVGView::getSource() {
    return src.c_str();
}

const char *SVGView::name() {
    return "SVG";
}

}
