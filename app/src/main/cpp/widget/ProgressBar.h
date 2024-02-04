//
// Created by ByteDance on 2022/8/5.
//

#ifndef SKIAUI_PROGRESSBAR_H
#define SKIAUI_PROGRESSBAR_H


#include "View.h"

class ProgressBar : public View {

public:

    enum class ProgressBarType {
        CIRCLE,
        LINEAR
    };

public:

    ProgressBar();

    virtual ~ProgressBar();

    virtual bool forceRequestTouchMove() override;

#pragma mark progressbar api

    virtual void setBarColor(SkColor color);

    virtual void setGradientBarColor(SkColor colors[], int size);

    virtual void setBackgroundColor(SkColor color) override;

    virtual void draw(SkCanvas *canvas) override;

    void measure(int widthMeasureSpec, int heightMeasureSpec) override;

    virtual void layout(int l, int t, int r, int b) override;

    /**
     * @param progress 0 - 100
     */
    virtual void setProgress(int progress);

    virtual void setAutoMode(bool autoMode);

    virtual void setType(ProgressBarType type);

    virtual bool onTouchEvent(TouchEvent *touchEvent) override;

    virtual void setProgressCallback(std::function<void(int progress)> progressCallback);

    virtual void setStrokeWidth(SkScalar _width) override;

protected:

    SkRect progressRect;

private:

    int index;

    bool autoMode;

    int progress;

    ProgressBarType type;

    bool pressed;

    std::function<void(int progress)> progressCallback;

    std::vector<SkColor> gradientColors;

    int gradientColorSize;

    bool setShader;

    SkPaint *backgroundPaint;

};


#endif //SKIAUI_PROGRESSBAR_H
