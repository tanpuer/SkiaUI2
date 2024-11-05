#include "RichText.h"
#include "json11.h"

RichText::RichText() : View() {
    defaultStyle = std::make_unique<TextStyle>();
    fontFamily.emplace_back("Alimama");
    fontFamily.emplace_back("ColorEmoji");
}

RichText::~RichText() {

}

void RichText::setText(SkString jsonValue) {
    std::string err;
    auto json = json11::Json::parse(jsonValue.c_str(), err);
    if(err.length() > 0){
        return;
    }
    const json11::Json::array& jsonArray = json.array_items();
    for (int32_t index = 0; index < static_cast<int>(jsonArray.size()); index++) {
        auto child = jsonArray[index];
    }
}

void RichText::measure() {
    //Todo Test
    if (nodes.empty()) {
        auto textNode = Node();
        textNode.type = NodeType::Txt;
        textNode.text = "这是测试标题";
        textNode.color = SK_ColorBLUE;
        textNode.underline = true;
        textNode.weight = 700;
        textNode.italic = true;
        nodes.push_back(textNode);

        auto imageNode = Node();
        imageNode.type = NodeType::Img;
        imageNode.width = 128;
        imageNode.height = 72;
        imageNode.src = "raining.png";
        nodes.push_back(imageNode);

        auto textNode1 = Node();
        textNode1.type = NodeType::Txt;
        textNode1.text = "这是测试普通文字";
        textNode1.color = SK_ColorBLACK;
        textNode1.fontSize = 50;
        textNode1.deleteText = true;
        nodes.push_back(textNode1);

        auto imageNode1 = Node();
        imageNode1.type = NodeType::Img;
        imageNode1.width = 128;
        imageNode1.height = 100;
        imageNode1.src = "transition2.png";
        nodes.push_back(imageNode1);

        auto textNode2 = Node();
        textNode2.type = NodeType::Txt;
        textNode2.text = "再来一张图片";
        textNode2.color = SK_ColorGREEN;
        textNode2.fontSize = 50;
        nodes.push_back(textNode2);
    }
    if (isDirty && paragraphWidth != width) {
        auto fontCollection = getContext()->getFontCollection();
        skia::textlayout::ParagraphStyle paraStyle;
        paraStyle.setTextStyle(*defaultStyle);
        paraStyle.setTextAlign(TextAlign::kLeft);
        paragraphBuilder = ParagraphBuilder::make(paraStyle, fontCollection);

        for (int i = 0; i < nodes.size(); ++i) {
            auto item = nodes[i];
            if (item.type == NodeType::Img) {
                PlaceholderStyle placeholderStyle(item.width, item.height,
                                                  PlaceholderAlignment::kAboveBaseline,
                                                  TextBaseline::kAlphabetic, 0);
                auto resourcesLoader = getContext()->resourcesLoader;
                resourcesLoader->decodeImage(item.src,
                                             [i, this](
                                                     const std::vector<sk_sp<SkImage>> &images,
                                                     sk_sp<SkAnimatedImage> animatedImage) {
                                                 auto image = images[0];
                                                 this->nodes[i].skImage = image;
                                             });
                TextStyle textStyle;
                paragraphBuilder->pushStyle(textStyle);
                paragraphBuilder->addPlaceholder(placeholderStyle);
            } else if (item.type == NodeType::Txt) {
                TextStyle textStyle;
                textStyle.setColor(item.color);
                SkFontStyle fontStyle(item.weight,
                                      SkFontStyle::kNormal_Width,
                                      item.italic ? SkFontStyle::Slant::kItalic_Slant
                                                  : SkFontStyle::Slant::kUpright_Slant);
                textStyle.setFontStyle(fontStyle);
                textStyle.setFontSize(item.fontSize);
                textStyle.setFontFamilies(fontFamily);
                if (item.underline) {
                    textStyle.setDecoration(TextDecoration::kUnderline);
                } else if (item.deleteText) {
                    textStyle.setDecoration(TextDecoration::kLineThrough);
                }
                paragraphBuilder->pushStyle(textStyle);
                paragraphBuilder->addText(item.text.c_str());
            }
        }
        paragraph = paragraphBuilder->Build();
        paragraph->layout(width);
        height = paragraph->getHeight();
        setMeasuredDimension(static_cast<int>(width), static_cast<int>(height));
        paragraphWidth = width;
        isDirty = false;
    }
}

void RichText::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (paragraph == nullptr) {
        return;
    }
    auto j = 0;
    for (int i = 0; i < nodes.size(); ++i) {
        auto item = nodes[i];
        if (item.type == NodeType::Img) {
            if (item.skImage == nullptr) {
                j++;
                continue;
            }
            auto rect = paragraph->getRectsForPlaceholders()[j].rect;
            canvas->drawImageRect(item.skImage, SkRect::MakeLTRB(rect.left(),
                                                                 rect.top() + skRect.top(),
                                                                 rect.right(),
                                                                 rect.bottom() + skRect.top()),
                                  SkSamplingOptions());
            j++;
        }
    }
    paragraph->paint(canvas, skRect.left(), skRect.top());
}

const char *RichText::name() {
    return "RichText";
}
