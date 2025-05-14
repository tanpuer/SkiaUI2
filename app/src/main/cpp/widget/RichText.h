#pragma once

#include "View.h"
#include "core/SkFont.h"
#include "AndroidBitmap.h"

namespace HYSkiaUI {

class RichText : public View {

    enum class NodeType {
        Txt,
        Img,
    };

    struct Node {
        NodeType type = NodeType::Txt;

        SkString text;
        SkColor color = SK_ColorBLACK;
        int fontSize = 100;
        bool underline = false;
        int weight = SkFontStyle::Weight::kNormal_Weight; //400=normal, 700 = bold
        bool italic = false; //slant
        bool deleteText = false;

        const char *src;
        int width = 0;
        int height = 0;
        sk_sp<SkImage> skImage = nullptr;
    };

public:

    RichText();

    ~RichText();

    void setText(const SkString &jsonValue);

    const char *name() override;

    void measure() override;

    void draw(SkCanvas *canvas) override;

    void onShow() override;

    void onHide() override;

private:

    std::unique_ptr<TextStyle> defaultStyle;

    std::unique_ptr<ParagraphBuilder> paragraphBuilder;

    std::unique_ptr<Paragraph> paragraph;

    std::vector<SkString> fontFamily;

    std::vector<Node> nodes;

    int paragraphWidth = 0;

    std::vector<std::unique_ptr<AndroidBitmap>> androidBitmaps;

};

}
