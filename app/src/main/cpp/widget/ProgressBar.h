#pragma once

#include "View.h"

namespace HYSkiaUI {

class ProgressBar : public View {

public:

    enum class ProgressBarType {
        CIRCLE,
        LINEAR
    };

public:

    ProgressBar();

    virtual ~ProgressBar();

    bool onInterceptTouchEvent(TouchEvent *touchEvent) override;

#pragma mark progressbar api

    const char *name() override;

    virtual void setBarColor(SkColor color);

    virtual void setBarColor(const std::string &hexColor);

    virtual const char *getBarColor();

    virtual void setGradientBarColor(SkColor colors[], int size);

    virtual void setBackgroundColor(SkColor color) override;

    virtual void draw(SkCanvas *canvas) override;

    void measure() override;

    virtual void layout(int l, int t, int r, int b) override;

    /**
     * @param progress 0 - 100
     */
    virtual void setProgress(float progress);

    virtual void setAutoMode(bool autoMode);

    virtual bool getAutoMode();

    virtual void setType(ProgressBarType type);

    virtual ProgressBarType getType();

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    virtual void
    setProgressCallback(std::function<void(int progress, bool finished)> progressCallback);

    virtual void setStrokeWidth(SkScalar _width) override;

protected:

    SkRect progressRect;

private:

    int index;

    bool autoMode;

    float progress;

    ProgressBarType type;

    bool pressed;

    std::function<void(int progress, bool finished)> progressCallback;

    std::vector<SkColor> gradientColors;

    int gradientColorSize;

    bool setShader;

    SkPaint *backgroundPaint;

    std::string barColor;

};

}
