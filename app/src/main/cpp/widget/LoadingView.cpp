#include "LoadingView.h"

namespace HYSkiaUI {

LoadingView::LoadingView() {
    paint = std::make_unique<SkPaint>();
    paint->setStyle(SkPaint::Style::kStrokeAndFill_Style);
    paint->setColor(SK_ColorGREEN);
}

LoadingView::~LoadingView() {
    ALOGD("LoadingView destroy");
}

void LoadingView::draw(SkCanvas *canvas) {
    auto time = getContext()->getCurrentTimeMills() % duration;
    for (int i = 0; i < size; ++i) {
        auto distance = i - size / 2;
        SkRect rect;
        auto startTime = i * duration / 2 / (size - 1);
        auto endTime = startTime + duration / 2;
        auto middleTime = startTime + duration / (size - 1);
        auto rectHeight = 0;
        if (time > endTime || time < startTime) {
            rectHeight = loadingHeight;
        } else {
            rectHeight = static_cast<int>(
                    std::min(abs(time - startTime), abs(time - endTime)) * loadingHeight * 2 /
                    (middleTime - startTime) + loadingHeight);
        }
        rect.setXYWH(static_cast<float >(left + width / 2 + loadingWidth * distance +
                                         loadingMargin * distance),
                     static_cast<float >(top + height / 2 - rectHeight / 2),
                     static_cast<float >(loadingWidth),
                     static_cast<float >(rectHeight));
        canvas->save();
        canvas->setMatrix(viewMatrix);
        canvas->drawRect(rect, *paint);
        canvas->restore();
    }
    markDirty();
}

void LoadingView::setLoadingColor(SkColor color) {
    paint->setColor(color);
}

void LoadingView::setLoadingWidth(int width) {
    loadingWidth = width;
}

void LoadingView::setSize(int size) {
    SkASSERT(size > 0);
    this->size = size;
}

void LoadingView::setLoadingHeight(int height) {
    loadingHeight = height;
}

void LoadingView::setLoadingDuration(int duration) {
    this->duration = duration;
}

const char *LoadingView::name() {
    return "Loading";
}

}
