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
    src = path;
    auto assetManager = getContext()->getAssetManager();
    auto imageData = assetManager->readImage(path);
    auto length = imageData->length;
    auto skData = SkData::MakeWithProc(imageData->content, length, nullptr, nullptr);
    auto stream = SkMemoryStream::Make(skData);
    skSVGDom = SkSVGDOM::Builder().make(*stream);
}

void SVGView::setText(const std::string &text) {
    auto stream = SkMemoryStream::MakeDirect(text.c_str(), text.size());
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
    if (skSVGDom != nullptr && (width != (r - l) || height != (t - b))) {
        skSVGDom->setContainerSize(SkSize::Make(r - l, t - b));
    }
    View::layout(l, t, r, b);
}

void SVGView::setXY(int x, int y) {
    this->x = x;
    this->y = y;
    isDirty = true;
}

const char *SVGView::getSource() {
    return src.c_str();
}
