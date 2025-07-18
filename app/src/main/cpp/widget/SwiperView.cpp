#include "SwiperView.h"

namespace HYSkiaUI {

SwiperView::SwiperView() {
    indicatorPaint = std::make_unique<SkPaint>();
    indicatorPaint->setAntiAlias(true);
    indicatorPaint->setStyle(SkPaint::Style::kFill_Style);
}

SwiperView::~SwiperView() {
    clearTimer();
}

void SwiperView::setAutoMode(bool flag) {
    this->autoMode = flag;
    if (autoMode) {
        if (timerId != -1L) {
            return;
        }
        timerId = getContext()->setTimer([this]() {
            auto targetIndex = currentIndex + 1;
            if (targetIndex >= children.size()) {
                targetIndex = 0;
            }
            scrollToIndex(targetIndex, true);
            if (currentIndex != targetIndex) {
                currentIndex = targetIndex;
                if (changeListener != nullptr) {
                    changeListener(currentIndex);
                }
            }
            markDirty();
        }, autoSwipeDuration, true);
    } else {
        clearTimer();
    }
}

void SwiperView::draw(SkCanvas *canvas) {
    ScrollView::draw(canvas);
    drawIndicators(canvas);
}

void SwiperView::drawIndicators(SkCanvas *canvas) {
    int size = children.size();
    if (size == 0) {
        return;
    }
    auto totalSize = (size - 1) * distance + size * indicatorSize;
    auto start = (width - totalSize) / 2;
    for (int i = 0; i < size; ++i) {
        auto centerX = left + start + (i + 0.5) * indicatorSize + i * distance;
        auto centerY = top + height * 0.8;
        indicatorPaint->setColor(i == currentIndex ? SK_ColorWHITE : SK_ColorGRAY);
        canvas->drawCircle(centerX, centerY, indicatorSize / 2, *indicatorPaint);
    }
}

void SwiperView::setContext(std::shared_ptr<SkiaUIContext>& context) {
    View::setContext(context);
    startTime = getContext()->getCurrentTimeMills();
}

void SwiperView::startFling() {
    auto targetIndex = findTargetIndex();
    scrollToIndex(targetIndex, true);
    if (currentIndex != targetIndex) {
        currentIndex = targetIndex;
        if (changeListener != nullptr) {
            changeListener(currentIndex);
        }
    }
    startTime = getContext()->getCurrentTimeMills();
}

int SwiperView::findTargetIndex() {
    auto value = (int) abs(translateX) % width;
    auto index = (int) abs(translateX) / width;
    if (value > width / 2) {
        index++;
    }
    return index;
}

void SwiperView::setOnChangeListener(std::function<void(int)> &&callback) {
    this->changeListener = std::move(callback);
}

bool SwiperView::onTouchEvent(TouchEvent *touchEvent) {
    if (touchEvent->action == TouchEvent::ACTION_DOWN) {
        lastAutoMode = this->autoMode;
        setAutoMode(false);
    } else if (touchEvent->action == TouchEvent::ACTION_UP ||
               touchEvent->action == TouchEvent::ACTION_CANCEL) {
        setAutoMode(lastAutoMode);
    }
    return ViewGroup::onTouchEvent(touchEvent);
}

void SwiperView::setSwipeDuration(long duration) {
    this->autoSwipeDuration = duration;
}

void SwiperView::clearTimer() {
    if (timerId != -1L) {
        getContext()->clearTimer(timerId);
        timerId = -1L;
    }
}

void SwiperView::onShow() {
    if (autoMode) {
        setAutoMode(true);
    }
    ViewGroup::onHide();
}

void SwiperView::onHide() {
    clearTimer();
    ViewGroup::onShow();
}

}
