#pragma once

#include "View.h"
#include "string"
#include "core/SkFont.h"
#include "skparagraph/include/TextStyle.h"
#include "skparagraph/include/ParagraphStyle.h"
#include "skparagraph/include/Paragraph.h"
#include "skparagraph/include/ParagraphBuilder.h"
#include "skparagraph/include/TypefaceFontProvider.h"

using namespace skia::textlayout;

class TextView : public View {

public:

    struct StringBuilder {
        SkString text;
        SkFontStyle fontStyle;
        SkScalar textSize;
        SkPaint foregroundPaint;
        SkString fontFamily;

        StringBuilder(SkString text, SkFontStyle fontStyle,
                      SkScalar textSize, SkPaint foregroundPaint, SkString fontFamily = SkString("Alimama")) noexcept {
            this->text = std::move(text);
            this->fontStyle = std::move(fontStyle);
            this->textSize = textSize;
            this->foregroundPaint = std::move(foregroundPaint);
            this->fontFamily = std::move(fontFamily);
        }

        ~StringBuilder() {

        }
    };


public:

    TextView();

    virtual ~TextView();

    void measure() override;

    virtual void draw(SkCanvas *canvas) override;

    const char *name() override;

#pragma mark TextView api

    virtual void setText(const char *text);

    virtual void setText(SkString text);

    virtual SkString getText();

    virtual void setTextColor(SkColor color);

    virtual void setTextSize(SkScalar textSize);

    virtual void setTextAlign(TextAlign textAlign);

    virtual SkScalar getTextSize();

    virtual void setAlpha(float alpha) override;

    void setMaxLines(int maxLine);

    /**
     * 功能类似于Android的StringBuilder
     * @param color
     * @param fontStyle
     * @param text
     */
    void pushText(const StringBuilder &stringBuilder);

    /**
     * 上/中/下 划线
     */
    void setDecoration(TextDecoration decoration);

    /**
     * 实线/虚线/波浪线/点
     */
    void setDecorationStyle(TextDecorationStyle style);

    void setDecorationColor(SkColor color);

    /**
     * decoration线条的粗度倍数
     */
    void setDecorationThicknessMultiplier(SkScalar m);

    /**
     * @param locale "en_US"
     */
    void setLocale(const SkString &locale);

    void addShadow(SkColor color, SkPoint offset, double blurSigma);

    /**
     * 设置线性渐变，比如歌词
     */
    void setTextGradient(std::vector<SkColor> colors, std::vector<float> pos);

    void setHeight(int height) override;

    void setFontFamily(const char *fontFamily);

    void setEllipsis(const char* ellipsis);

protected:

    SkString text;

    std::vector<SkString> textVector;

    std::unique_ptr<TextStyle> defaultStyle;

    std::unique_ptr<ParagraphBuilder> paragraphBuilder;

    std::unique_ptr<Paragraph> paragraph;

    int maxLine;

    SkColor skColor;

    std::vector<StringBuilder> stringBuilders;

    std::vector<SkColor> textGradientColors;

    std::vector<float> textGradientPos;

    int originHeight = 0;

    std::vector<SkString> fontFamily;

    TextAlign textAlign = TextAlign::kLeft;

    SkString ellipsis = SkString("\u2026");
};
