#include "ClockView.h"
#include "core/SkPath.h"
#include "ports/SkFontMgr_android.h"
#include "core/SkFontMgr.h"
#include "core/SkFontStyle.h"
#include "core/SkTypeface.h"
#include "core/SkFont.h"

namespace HYSkiaUI {

ClockView::ClockView() {
    numberPaint = std::make_unique<SkPaint>();
    numberPaint->setAntiAlias(true);
    numberPaint->setColor(SK_ColorBLACK);
    auto fontMgr = SkFontMgr_New_Android(nullptr);
    SkString familyName;
    fontMgr->getFamilyName(0, &familyName);
    font = std::make_unique<SkFont>(
            fontMgr->legacyMakeTypeface(familyName.c_str(), SkFontStyle::Bold()));
    font->setSize(60);

    circlePaint = std::make_unique<SkPaint>();
    circlePaint->setColor(SK_ColorBLACK);
    circlePaint->setStyle(SkPaint::kStroke_Style);
    circlePaint->setAntiAlias(true);
    circlePaint->setStrokeWidth(5);

    detailPaint = std::make_unique<SkPaint>();
    detailPaint->setAntiAlias(true);
    detailPaint->setColor(SK_ColorBLACK);
    detailPaint->setStyle(SkPaint::kStroke_Style);

    centerPaint = std::make_unique<SkPaint>();
    centerPaint->setColor(SK_ColorBLACK);
    centerPaint->setStyle(SkPaint::kFill_Style);
    centerPaint->setAntiAlias(true);

    handPaint = std::make_unique<SkPaint>();
    handPaint->setStyle(SkPaint::kStroke_Style);
    handPaint->setAntiAlias(true);
}

ClockView::~ClockView() {

}

void ClockView::draw(SkCanvas *canvas) {
    drawBackground(canvas);

    std::time_t now = std::time(nullptr);
    std::tm *now_tm = std::localtime(&now);
    ALOGD("ClockView::currentTime %d %d %d", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec)
    // 时针
    drawClockHand(canvas, (now_tm->tm_hour % 12 * 30) + (now_tm->tm_min * 0.5),
                  skRect.width() * 0.2, 20, SK_ColorBLUE);
    // 分针
    drawClockHand(canvas, now_tm->tm_min * 6 + now_tm->tm_sec * 0.1, skRect.width() * 0.3, 10,
                  SK_ColorYELLOW);
    // 秒针
    drawClockHand(canvas, now_tm->tm_sec * 6, skRect.width() * 0.4, 5, SK_ColorRED);

    canvas->drawCircle(skRect.left() + skRect.width() / 2, skRect.top() + skRect.height() / 2, 20,
                       *centerPaint);
}

void ClockView::drawClockHand(SkCanvas *canvas, float angle, float length, float handWidth,
                              SkColor color) {
    handPaint->setColor(color);
    handPaint->setStrokeWidth(handWidth);
    float radians = angle * M_PI / 180;
    SkPath path;
    path.moveTo(skRect.left() + skRect.width() / 2,
                skRect.top() + skRect.height() / 2);
    path.lineTo(skRect.left() + skRect.width() / 2 + length * cos(radians - M_PI / 2),
                skRect.top() + skRect.height() / 2 + length * sin(radians - M_PI / 2));
    canvas->drawPath(path, *handPaint);
}

void ClockView::drawBackground(SkCanvas *canvas) {
    canvas->drawCircle(skRect.left() + skRect.width() / 2, skRect.top() + skRect.height() / 2,
                       skRect.width() * 0.5, *circlePaint);
    detailPaint->setStrokeWidth(3);
    detailPaint->setAlphaf(1.0);
    auto width = skRect.width();
    auto radius = width / 2;
    for (int i = 0; i < 12; ++i) {
        SkPath path;
        float radians = i * 30 * M_PI / 180;
        auto x = skRect.left() + skRect.width() / 2 + radius * cos(radians - M_PI / 2);
        auto y = skRect.top() + skRect.height() / 2 + radius * sin(radians - M_PI / 2);
        path.moveTo(x, y);
        auto x1 = skRect.left() + skRect.width() / 2 + radius * 0.8 * cos(radians - M_PI / 2);
        auto y1 = skRect.top() + skRect.height() / 2 + radius * 0.8 * sin(radians - M_PI / 2);
        path.lineTo(x1, y1);
        canvas->drawPath(path, *detailPaint);
    }
    detailPaint->setAlphaf(0.5);
    detailPaint->setStrokeWidth(1.5);
    for (int i = 0; i < 60; ++i) {
        if (i % 5 == 0) {
            continue;
        }
        SkPath path;
        float radians = i * 6 * M_PI / 180;
        auto x = skRect.left() + skRect.width() / 2 + radius * cos(radians - M_PI / 2);
        auto y = skRect.top() + skRect.height() / 2 + radius * sin(radians - M_PI / 2);
        path.moveTo(x, y);
        auto x1 = skRect.left() + skRect.width() / 2 + radius * 0.9 * cos(radians - M_PI / 2);
        auto y1 = skRect.top() + skRect.height() / 2 + radius * 0.9 * sin(radians - M_PI / 2);
        path.lineTo(x1, y1);
        canvas->drawPath(path, *detailPaint);
    }
    for (int i = 0; i < 12; ++i) {
        float radians = (i * 30) * M_PI / 180;
        auto x = skRect.left() + skRect.width() / 2 + radius * 0.7 * cos(radians - M_PI / 2) - 20;
        auto y = skRect.top() + skRect.height() / 2 + radius * 0.7 * sin(radians - M_PI / 2) + 20;
        canvas->drawString(SkString(std::to_string(i)), x, y, *font, *numberPaint);
    }
}

}
