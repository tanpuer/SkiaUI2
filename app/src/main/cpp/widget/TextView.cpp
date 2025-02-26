#include "TextView.h"

#include <utility>
#include <base/native_log.h>
#include "core/SkFont.h"
#include "ports/SkFontMgr_android.h"
#include "effects/SkGradientShader.h"
#include "skparagraph/include/TypefaceFontProvider.h"
#include "color_util.h"

namespace HYSkiaUI {

TextView::TextView() : View(), maxLine(0), skColor(SK_ColorBLACK) {
    defaultStyle = std::make_unique<TextStyle>();
    fontFamily.emplace_back("Alimama");
    fontFamily.emplace_back("ColorEmoji");
}

TextView::~TextView() {
    stringBuilders.clear();
}

const char *TextView::name() {
    return "TextView";
}

void TextView::setText(const char *text) {
    setText(SkString(text));
}

void TextView::setText(SkString text) {
    this->text = std::move(text);
    stringBuilders.clear();
    markDirty();
}

SkString TextView::getText() {
    return text;
}

void TextView::setTextColor(SkColor color) {
    skColor = color;
    defaultStyle->setColor(color);
    markDirty();
    markMeasure();
}

void TextView::setAlpha(float alpha) {
    defaultStyle->setColor(SkColorSetARGB(alpha * 255, SkColorGetR(skColor), SkColorGetG(skColor),
                                          SkColorGetB(skColor)));
    markDirty();
    markMeasure();
}

float TextView::getAlpha() {
    return SkColorGetA(defaultStyle->getColor()) / 255.0f;
}

void TextView::measure() {
    if (text.isEmpty() && stringBuilders.empty()) {
        setMeasuredDimension(0, 0);
        return;
    }
    if (needToMeasure) {
        clearDirty();
        skia::textlayout::ParagraphStyle paraStyle;
        paraStyle.setTextStyle(*defaultStyle);
        paraStyle.setTextAlign(textAlign);
        if (maxLine > 0) {
            paraStyle.setEllipsis(ellipsis);
            paraStyle.setMaxLines(maxLine);
        }
        auto fontCollection = getContext()->getFontCollection();
        paragraphBuilder = ParagraphBuilder::make(paraStyle, fontCollection);
        if (!stringBuilders.empty()) {
            for (const auto &iterator: stringBuilders) {
                TextStyle textStyle;
                textStyle.setFontStyle(iterator.fontStyle);
                textStyle.setFontSize(iterator.textSize);
                textStyle.setFontFamilies({iterator.fontFamily});
                textStyle.setForegroundPaint(iterator.foregroundPaint);
                paragraphBuilder->pushStyle(textStyle);
                paragraphBuilder->addText(iterator.text.c_str());
            }
        } else {
            TextStyle textStyle;
            textStyle.setColor(defaultStyle->getColor());
            textStyle.setFontStyle(defaultStyle->getFontStyle());
            textStyle.setFontSize(getTextSize());
            textStyle.setFontFamilies(fontFamily);
            if (!textGradientColors.empty()) {
                SkPaint foregroundPaint;
                SkPoint points[2]{
                        SkPoint::Make(left, top), SkPoint::Make(right, top)
                };
                auto gradientShader = SkGradientShader::MakeLinear(
                        points,
                        textGradientColors.data(),
                        textGradientPos.data(),
                        textGradientColors.size(),
                        SkTileMode::kClamp
                );
                foregroundPaint.setShader(std::move(gradientShader));
                textStyle.setForegroundPaint(foregroundPaint);
            }
            paragraphBuilder->pushStyle(textStyle);
            paragraphBuilder->addText(text.c_str());
        }
        paragraph = paragraphBuilder->Build();
        auto width = 0.0f;
        auto height = 0.0f;
        //为了方便计算大小，最好强制制定TextView的宽度，否则默认用maxIntrinsicWidth
        if (originHeight > 0 && this->originWidth > 0) {
            width = this->originWidth;
            paragraph->layout(this->originWidth);
        } else {
            paragraph->layout(SK_ScalarInfinity);
            width = paragraph->getMaxIntrinsicWidth() + 1;
            paragraph->layout(width);
        }
        auto spacing = defaultStyle->getWordSpacing();
        if (originHeight > 0 && spacing > 0) {
            // Parent has told us how big to be. So be it.
            height = static_cast<float >(this->height);
            if (paragraph->getHeight() > this->height) {
                setMaxLines(floor(this->height / spacing));
                //当发现文字高度大于textview高度，更新maxLine，重新走measure方法
                measure();
                return;
            }
        } else {
            height = paragraph->getHeight();
        }
        setMeasuredDimension(static_cast<int>(width), static_cast<int>(height));
        ALOGD("TextView setSize %f %f", width, height)
        clearMeasure();
    }
}

void TextView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (paragraph == nullptr) {
        return;
    }
    paragraph->paint(canvas, skRect.left(), skRect.top());
}

void TextView::setTextSize(SkScalar textSize) {
    defaultStyle->setFontSize(textSize);
    markDirty();
    markMeasure();
}

void TextView::setTextAlign(TextAlign textAlign) {
    this->textAlign = textAlign;
    markDirty();
    markMeasure();
}

void TextView::setMaxLines(int maxLine) {
    this->maxLine = maxLine;
    markDirty();
    markMeasure();
}

void TextView::setDecoration(TextDecoration decoration) {
    defaultStyle->setDecoration(decoration);
}

void TextView::setDecorationStyle(TextDecorationStyle style) {
    defaultStyle->setDecorationStyle(style);
}

void TextView::setDecorationColor(SkColor color) {
    defaultStyle->setDecorationColor(color);
}

void TextView::setDecorationThicknessMultiplier(SkScalar m) {
    defaultStyle->setDecorationThicknessMultiplier(m);
}

void TextView::setLocale(const SkString &locale) {
    defaultStyle->setLocale(locale);
}

void TextView::addShadow(SkColor color, SkPoint offset, double blurSigma) {
    auto shadow = TextShadow(color, offset, blurSigma);
    defaultStyle->addShadow(shadow);
}

void TextView::pushText(const TextView::StringBuilder &stringBuilder) {
    stringBuilders.emplace_back(stringBuilder);
    markDirty();
    markMeasure();
}

SkScalar TextView::getTextSize() {
    return defaultStyle->getFontSize();
}

void TextView::setTextGradient(std::vector<SkColor> colors, std::vector<float> pos) {
    textGradientColors = std::move(colors);
    textGradientPos = std::move(pos);
    markDirty();
    markMeasure();
}

void TextView::setHeight(int height) {
    View::setHeight(height);
    originHeight = height;
}

void TextView::setWidth(int width) {
    View::setWidth(width);
    originWidth = width;
}

void TextView::setFontFamily(const char *fontFamily) {
    this->fontFamily.clear();
    this->fontFamily.emplace_back(fontFamily);
    markDirty();
    markMeasure();
}

void TextView::setEllipsis(const char *ellipsis) {
    this->ellipsis = ellipsis;
    markDirty();
    markMeasure();
}

void TextView::setTextColor(const std::string &hexColor) {
    textColor = hexColor;
    int r, g, b, a;
    hexToRGBA(hexColor, r, g, b, a);
    setTextColor(SkColorSetARGB(a, r, g, b));
}

const char *TextView::getTextColor() {
    return textColor.c_str();
}

}
