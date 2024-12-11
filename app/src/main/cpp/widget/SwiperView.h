#pragma once

#include "ScrollView.h"

namespace HYSkiaUI {

class SwiperView : public ScrollView {

public:

    SwiperView();

    ~SwiperView();

    virtual void setAutoMode(bool flag);

    void setContext(std::shared_ptr<SkiaUIContext> context) override;

    virtual void draw(SkCanvas *canvas) override;

    virtual void drawIndicators(SkCanvas *canvas);

    void startFling() override;

    void setOnChangeListener(std::function<void(int)>&& callback);

    bool onTouchEvent(HYSkiaUI::TouchEvent *touchEvent) override;

    void setSwipeDuration(long duration);

private:

    int findTargetIndex();

    bool autoMode = false;

    std::unique_ptr<SkPaint> indicatorPaint;

    int indicatorSize = 20;

    int currentIndex = 0;

    int distance = 40;

    std::function<void(int)> changeListener = nullptr;

    bool swipeFlag = true;

    long startTime = 0L;

    long autoSwipeDuration = 5000L;

};

}
