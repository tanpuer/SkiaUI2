#include "TextView.h"

#include <utility>
#include <base/native_log.h>
#include "core/SkFont.h"
#include "math.h"
#include "ports/SkFontMgr_android.h"

TextView::TextView() : View(), maxLine(0), skColor(SK_ColorBLACK) {
    textRect = SkRect::MakeEmpty();
    defaultStyle = std::make_unique<TextStyle>();
    fontCollection = sk_make_sp<FontCollection>();
    auto fontMgr = SkFontMgr_New_Android(nullptr);
    fontCollection->setDefaultFontManager(fontMgr);
    stringBuilders = std::vector<StringBuilder>();
    SkString familyName;
    fontMgr->getFamilyName(0, &familyName);
    font = std::make_unique<SkFont>(fontMgr->legacyMakeTypeface(familyName.c_str(), SkFontStyle::Normal()));
}

TextView::~TextView() {
    stringBuilders.clear();
}

const char *TextView::name() {
    return "TextView";
}

void TextView::setText(SkString text) {
    this->text = std::move(text);
    stringBuilders.clear();
    isDirty = true;
}

SkString TextView::getText() {
    return text;
}

void TextView::setTextColor(SkColor color) {
    skColor = color;
    defaultStyle->setColor(color);
    isDirty = true;
}

void TextView::setAlpha(float alpha) {
    View::setAlpha(alpha);
    defaultStyle->setColor(SkColorSetARGB(alpha * 255, SkColorGetR(skColor), SkColorGetG(skColor),
                                          SkColorGetB(skColor)));
    isDirty = true;
}

void TextView::measure() {
    if (text.isEmpty() && stringBuilders.empty()) {
        setMeasuredDimension(0, 0);
        return;
    }
    if (isDirty) {
        isDirty = false;
        skia::textlayout::ParagraphStyle paraStyle;
        paraStyle.setTextStyle(*defaultStyle);
        paraStyle.setTextAlign(TextAlign::kCenter);
        if (maxLine > 0) {
            paraStyle.setEllipsis(u"\u2026");
            paraStyle.setMaxLines(maxLine);
        }
        paragraphBuilder = ParagraphBuilder::make(paraStyle, fontCollection);
        if (!stringBuilders.empty()) {
            for (const auto &iterator: stringBuilders) {
                TextStyle textStyle;
                textStyle.setColor(iterator.color);
                textStyle.setFontStyle(iterator.fontStyle);
                textStyle.setFontSize(iterator.textSize);
                paragraphBuilder->pushStyle(textStyle);
                paragraphBuilder->addText(iterator.text.c_str());
            }
        } else {
            paragraphBuilder->addText(text.c_str());
        }
        paragraph = paragraphBuilder->Build();
        auto width = 0.0f;
        auto height = 0.0f;
        //为了方便计算大小，最好强制制定TextView的宽度，否则默认用maxIntrinsicWidth
        if (this->width > 0) {
            width = this->width;
            paragraph->layout(this->width);
        } else {
            paragraph->layout(SK_ScalarInfinity);
            width = paragraph->getMaxIntrinsicWidth() + 1;
            paragraph->layout(width);
        }
        auto spacing = font->getSpacing();
        if (this->height > 0 && spacing > 0) {
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
    font->setSize(textSize);
    defaultStyle->setFontSize(textSize);
    isDirty = true;
}

void TextView::setMaxLines(int maxLine) {
    assert(maxLine > 0);
    this->maxLine = maxLine;
    isDirty = true;
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
    isDirty = true;
}
