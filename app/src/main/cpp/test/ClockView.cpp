#include "ClockView.h"
#include "core/SkPath.h"

ClockView::ClockView() {

}

ClockView::~ClockView() {

}

void ClockView::draw(SkCanvas *canvas) {
    // 绘制时钟圆盘
    SkPaint paintCircle;
    paintCircle.setColor(SK_ColorBLACK);
    paintCircle.setStyle(SkPaint::kStroke_Style);
    paintCircle.setAntiAlias(true);
    paintCircle.setStrokeWidth(2);
    canvas->drawCircle(skRect.left() + skRect.width() / 2, skRect.top() + skRect.height() / 2,
                       skRect.width() * 0.5, paintCircle);
    std::time_t now = std::time(nullptr);
    std::tm *now_tm = std::localtime(&now);
    ALOGD("ClockView::currentTime %d %d %d", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec)
    // 时针
    drawClockHand(canvas, (now_tm->tm_hour % 12 * 30) + (now_tm->tm_min * 0.5),
                  skRect.width() * 0.2, 20, SK_ColorBLACK);
    // 分针
    drawClockHand(canvas, now_tm->tm_min * 6, skRect.width() * 0.3, 10, SK_ColorYELLOW);
    // 秒针
    drawClockHand(canvas, now_tm->tm_sec * 6, skRect.width() * 0.4, 5, SK_ColorRED);
}

void ClockView::drawClockHand(SkCanvas *canvas, float angle, float length, float handWidth,
                              SkColor color) {
    SkPaint paint;
    paint.setColor(color);
    paint.setStrokeWidth(handWidth);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setAntiAlias(true);
    float radians = angle * M_PI / 180;
    SkPath path;
    path.moveTo(skRect.left() + skRect.width() / 2,
                skRect.top() + skRect.height() / 2);
    path.lineTo(skRect.left() + skRect.width() / 2 + length * cos(radians),
                skRect.top() + skRect.height() / 2 + length * sin(radians));
    canvas->drawPath(path, paint);
}
