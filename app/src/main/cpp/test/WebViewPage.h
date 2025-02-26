#pragma once

#include "Page.h"
#include "LinearAnimator.h"

namespace HYSkiaUI {

class WebViewProgressView : public View {

public:

    WebViewProgressView() {
        paint->setColor(SK_ColorGREEN);
        progressRect.setXYWH(0, 0, 0, 0);
    }

    void setContext(std::shared_ptr<SkiaUIContext>& context) override {
        View::setContext(context);
        setStyle(SkPaint::Style::kFill_Style);
    }

    void draw(SkCanvas *canvas) override {
        progressRect.setXYWH(left, top, width * progress / 100, getHeight());
        canvas->drawRect(progressRect, *paint);
    }

    void setProgress(int progress) {
        if (progress < 0 || progress > 100) {
            return;
        }
        this->progress = progress;
        markDirty();
        if (progress == 100) {
            auto animator = new LinearAnimator(this, 1.0, 0.0);
            animator->setDuration(500);
            animator->setUpdateListener([](View *view, float value) {
                view->setAlpha(value);
            });
            animator->start();
        } else {
            setAlpha(1.0f);
        }
    }

private:

    SkRect progressRect;

    int progress = 0;
};

class WebViewPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

    WebViewProgressView *progressView = nullptr;

};

}
