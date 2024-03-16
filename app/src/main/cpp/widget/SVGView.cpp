//
// Created by banma-3412 on 2024/3/14.
//

#include "SVGView.h"
#include "core/SkStream.h"
#include "svg/include/SkSVGDOM.h"
#include "SkiaUIContext.h"

SVGView::SVGView() {
    paint->setColor(SK_ColorWHITE);
}

SVGView::~SVGView() {

}

void SVGView::setSource(const char *path) {
    auto assetManager = SkiaUIContext::getInstance()->getAssetManager();
    auto imageData = assetManager->readImage(path);
    auto length = imageData->length;
    auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
    auto stream = SkMemoryStream::Make(skData);
    skSVGDom = SkSVGDOM::Builder().make(*stream);
}

void SVGView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (skSVGDom != nullptr) {
        canvas->save();
        canvas->translate(left + x, top + y);
        skSVGDom->render(canvas);
        canvas->restore();
    }
}

void SVGView::layout(int l, int t, int r, int b) {
    if (width != (r - l) || height != (t - b)) {
        skSVGDom->setContainerSize(SkSize::Make(r - l, t - b));
    }
    View::layout(l, t, r, b);
}

void SVGView::setXY(int x, int y) {
    this->x = x;
    this->y = y;
    isDirty = true;
}
