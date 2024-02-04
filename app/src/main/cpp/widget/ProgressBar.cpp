//
// Created by ByteDance on 2022/8/5.
//

#include <core/SkPath.h>
#include "ProgressBar.h"
#include "effects/SkGradientShader.h"

ProgressBar::ProgressBar()
        : View(), progressRect(SkRect::MakeEmpty()), autoMode(true), progress(0), index(0),
          type(ProgressBarType::CIRCLE), pressed(false), progressCallback(nullptr) {
    paint->setStroke(true);
    gradientColors = std::vector<SkColor>();
    gradientColorSize = 0;
    setShader = false;
    backgroundPaint = new SkPaint();
    backgroundPaint->setAntiAlias(true);
    backgroundPaint->setStyle(SkPaint::kStroke_Style);
}

ProgressBar::~ProgressBar() {
    delete backgroundPaint;
}

void ProgressBar::setBarColor(SkColor color) {
    paint->setColor(color);
}

void ProgressBar::setGradientBarColor(SkColor colors[], int size) {
    // todo 滑动的时候centerX，centerY一直在变化，需要不停更新shader，因此gradient还是适合非滑动页面
    for (int i = 0; i < size; ++i) {
        gradientColors.emplace_back(colors[i]);
    }
    gradientColorSize = size;
}

void ProgressBar::setBackgroundColor(SkColor color) {
    backgroundPaint->setColor(color);
}

void ProgressBar::measure(int widthMeasureSpec, int heightMeasureSpec) {
    View::measure(widthMeasureSpec, heightMeasureSpec);
}

void ProgressBar::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    if (type == ProgressBarType::CIRCLE) {
        auto diff = width * 0.2;
        progressRect.setLTRB(l + diff, t + diff, r - diff, b - diff);
        if (!setShader && gradientColorSize > 0) {
            setShader = true;
            paint->setShader(
                    SkGradientShader::MakeSweep(progressRect.centerX(), progressRect.centerY(),
                                                &gradientColors[0], nullptr, gradientColorSize));
        }
    } else if (type == ProgressBarType::LINEAR) {
        auto diff = height / 3.0f;
        progressRect.setLTRB(l + marginLeft, t + diff, r - marginRight, b - diff);
        if (!setShader && gradientColorSize > 0) {
            setShader = true;
            std::vector<SkPoint> points;
            points.emplace_back(SkPoint::Make(progressRect.left(), progressRect.centerY()));
            points.emplace_back(SkPoint::Make(progressRect.right(), progressRect.centerY()));
            paint->setShader(
                    SkGradientShader::MakeLinear(&points[0], &gradientColors[0], nullptr,
                                                 gradientColorSize, SkTileMode::kClamp));
        }
    }
}

void ProgressBar::draw(SkCanvas *canvas) {
    if (type == ProgressBarType::CIRCLE) {
        if (autoMode) {
            index += 3;
            if (index > 360) {
                index = 0;
            }
        } else {
            index = progress * 360 / 100;
        }
        canvas->drawArc(progressRect, 0.0, 360, false, *backgroundPaint);
        canvas->drawArc(progressRect, 0.0, index, false, *paint);
    } else if (type == ProgressBarType::LINEAR) {
        if (autoMode) {
            progress += 1;
            if (progress > 100) {
                progress = 0;
            }
        }
        paint->setStyle(SkPaint::kFill_Style);
        backgroundPaint->setStyle(SkPaint::kFill_Style);
        canvas->drawRoundRect(progressRect, height / 2, height / 2, *backgroundPaint);
        progressRect.setLTRB(progressRect.left(), progressRect.top(),
                             progressRect.width() * progress / 100 + progressRect.left(),
                             progressRect.bottom());
        canvas->drawRoundRect(progressRect, height / 2, height / 2, *paint);
        canvas->drawCircle(progressRect.right(), progressRect.centerY(),
                           height / 3 * (pressed ? 1.2f : 1.0f), *paint);
    }
}

void ProgressBar::setProgress(int progress) {
    SkASSERT(progress <= 100 && progress >= 0);
    this->progress = progress;
    isDirty = true;
}

void ProgressBar::setAutoMode(bool autoMode) {
    this->autoMode = autoMode;
    isDirty = true;
}

void ProgressBar::setType(ProgressBar::ProgressBarType type) {
    this->type = type;
    isDirty = true;
}

bool ProgressBar::onTouchEvent(TouchEvent *touchEvent) {
    if (!autoMode && type == ProgressBarType::LINEAR) {
        switch (touchEvent->action) {
            case TouchEvent::ACTION_DOWN:
            case TouchEvent::ACTION_MOVE: {
                pressed = true;
                auto lastProgress = progress;
                progress = static_cast<int>((touchEvent->x - YGNodeLayoutGetLeft(node) - marginLeft)
                                            * 100 / (width - marginLeft - marginRight));
                if (progress > 100) {
                    progress = 100;
                } else if (progress < 0) {
                    progress = 0;
                }
                if (progressCallback != nullptr && lastProgress != progress) {
                    progressCallback(progress);
                }
                break;
            }
            case TouchEvent::ACTION_CANCEL:
            case TouchEvent::ACTION_UP: {
                pressed = false;
                break;
            }
        }
        return true;
    }
    return View::onTouchEvent(touchEvent);
}

void ProgressBar::setProgressCallback(std::function<void(int)> progressCallback) {
    this->progressCallback = progressCallback;
}

void ProgressBar::setStrokeWidth(SkScalar _width) {
    View::setStrokeWidth(_width);
    backgroundPaint->setStrokeWidth(_width);
}

bool ProgressBar::forceRequestTouchMove() {
    return true;
}
