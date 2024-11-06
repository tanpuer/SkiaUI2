#include "PickerView.h"
#include "effects/SkGradientShader.h"

PickerView::PickerView() {
    bottomPaint = std::make_unique<SkPaint>();
    bottomPaint->setAntiAlias(true);
    centerPaint = std::make_unique<SkPaint>();
    centerPaint->setAntiAlias(true);
    centerPaint->setColor(SkColorSetARGB(0x33, 0x88, 0x88, 0x88));
    maskColor.push_back(SkColorSetARGB(0xCC, 0xFF, 0xFF, 0xFF));
    maskColor.push_back(SkColorSetARGB(0x33, 0xFF, 0xFF, 0xFF));
    maskColor.push_back(SkColorSetARGB(0xCC, 0xFF, 0xFF, 0xFF));
}

PickerView::~PickerView() {

}

void PickerView::setContext(std::shared_ptr<SkiaUIContext> context) {
    View::setContext(context);
//    setJustifyContent(YGJustify::YGJustifyCenter);
    setAlignItems(YGAlign::YGAlignCenter);
}

void PickerView::setOnChangeListener(std::function<void(int)> &&callback) {
    this->callback = std::move(callback);
}

void PickerView::onFlingStopped() {
    scrollToNearestChild();
}

void PickerView::scrollToNearestChild() {

}

void PickerView::draw(SkCanvas *canvas) {
    ScrollView::draw(canvas);
//    canvas->drawRect(centerMask, *centerPaint);
    canvas->drawIRect(skRect, *bottomPaint);
}

void PickerView::measure() {
    SkAssertResult(width > 0);
    ScrollView::measure();
    if (isDirty) {
        auto totalHeight = 0;
        auto size = children.size() <= 4 ? children.size() : 4;
        for (int i = 0; i < size; ++i) {
            totalHeight += children[i]->getHeight();
        }
        ViewGroup::setMeasuredDimension(width, totalHeight);
        isDirty = false;
        centerHeight = children[0]->getHeight();
    }
}

void PickerView::layout(int l, int t, int r, int b) {
    ScrollView::layout(l, t, r, b);
    SkPoint points[2]{SkPoint::Make(l, t), SkPoint::Make(l, b)};
    auto shader = SkGradientShader::MakeLinear(
            points,
            maskColor.data(),
            nullptr,
            3,
            SkTileMode::kClamp
    );
    bottomPaint->setShader(std::move(shader));
    centerMask.setLTRB(skRect.left(), skRect.top() + skRect.height() / 2 - centerHeight / 2,
                       skRect.right(), skRect.top() + skRect.height() / 2 + centerHeight / 2);
}

bool PickerView::forceRequestTouchMove() {
    return true;
}
