#pragma once

#include "ScrollView.h"

class PickerView : public ScrollView {

public:

    PickerView();

    ~PickerView();

    void setContext(std::shared_ptr<SkiaUIContext> context) override;

    void setOnChangeListener(std::function<void(int)> &&callback);

    void onFlingStopped() override;

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    bool forceRequestTouchMove() override;

private:

    void scrollToNearestChild();

    std::function<void(int)> changeCallback;

    std::unique_ptr<SkPaint> bottomPaint;

    SkRect centerMask;

    std::unique_ptr<SkPaint> centerPaint;

    int centerHeight = 0;

    std::vector<SkColor> maskColor;
};
