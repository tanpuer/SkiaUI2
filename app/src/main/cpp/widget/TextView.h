#pragma once

#include "View.h"
#include "string"
#include "core/SkFont.h"
#include "skparagraph/include/TextStyle.h"
#include "skparagraph/include/ParagraphStyle.h"
#include "skparagraph/include/Paragraph.h"
#include "skparagraph/include/ParagraphBuilder.h"

using namespace skia::textlayout;

class TextView : public View {

public:

    struct StringBuilder {
        SkString text;
        SkColor color;
        SkFontStyle fontStyle;
        SkScalar textSize;

        StringBuilder(SkString text, SkColor color, SkFontStyle fontStyle,
                      SkScalar textSize) noexcept {
            this->text = std::move(text);
            this->color = color;
            this->fontStyle = std::move(fontStyle);
            this->textSize = textSize;
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

    virtual void setText(const char* text);

    virtual void setText(SkString text);

    virtual SkString getText();

    virtual void setTextColor(SkColor color);

    virtual void setTextSize(SkScalar textSize);

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

protected:

    SkString text;

    std::unique_ptr<SkFont> font;

    SkRect textRect;

    std::vector<SkString> textVector;

    std::unique_ptr<TextStyle> defaultStyle;

    std::unique_ptr<ParagraphBuilder> paragraphBuilder;

    sk_sp<FontCollection> fontCollection;

    std::unique_ptr<Paragraph> paragraph;

    int maxLine;

    SkColor skColor;

    std::vector<StringBuilder> stringBuilders;

};
