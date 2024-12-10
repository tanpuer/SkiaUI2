#include "InputView.h"

namespace HYSkiaUI {


InputView::InputView() {
    defaultStyle = std::make_unique<TextStyle>();
    fontFamily.emplace_back("Alimama");
    fontFamily.emplace_back("ColorEmoji");
    cursorPaint = std::make_unique<SkPaint>();
    cursorPaint->setColor(SK_ColorGREEN);
    cursorPaint->setAntiAlias(true);
    cursorPaint->setStyle(SkPaint::Style::kFill_Style);
    setOnClickListener([this](View *view) {
        auto jniEnv = getContext()->getJniEnv();
        auto jStr = jniEnv->NewStringUTF(this->content.c_str());
        jniEnv->CallVoidMethod(javaKeyboardRef, javaShowMethodId, jStr);
    });
}

InputView::~InputView() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaKeyboardRef, releaseMethodId);
    jniEnv->DeleteGlobalRef(javaKeyboardRef);
    javaKeyboardRef = nullptr;
}

void InputView::setPlaceHolder(const char *placeHolder) {
    this->placeHolder = placeHolder;
    markDirty();
}

void InputView::measure() {
    if (isDirty) {
        auto fontCollection = getContext()->getFontCollection();
        skia::textlayout::ParagraphStyle paraStyle;
        paraStyle.setTextStyle(*defaultStyle);
        paraStyle.setTextAlign(TextAlign::kLeft);
        paragraphBuilder = ParagraphBuilder::make(paraStyle, fontCollection);

        TextStyle textStyle;
        textStyle.setColor(content.empty() ? SK_ColorGRAY : SK_ColorBLACK);
        SkFontStyle fontStyle(SkFontStyle::Weight::kNormal_Weight,
                              SkFontStyle::kNormal_Width,
                              SkFontStyle::Slant::kUpright_Slant);
        textStyle.setFontStyle(fontStyle);
        textStyle.setFontSize(this->fontSize);
        textStyle.setFontFamilies(fontFamily);
        paragraphBuilder->pushStyle(textStyle);
        paragraphBuilder->addText(content.empty() ? placeHolder.c_str() : content.c_str());

        if (cursorHeight != 0) {
            TextStyle cursorStyle;
            paragraphBuilder->pushStyle(cursorStyle);
            PlaceholderStyle placeholderStyle(20, cursorHeight,
                                              PlaceholderAlignment::kAboveBaseline,
                                              TextBaseline::kAlphabetic, 0);
            paragraphBuilder->addPlaceholder(placeholderStyle);
        }

        paragraph = paragraphBuilder->Build();
        paragraph->layout(width);
        height = paragraph->getHeight();
        if (cursorHeight == 0) {
            cursorHeight = paragraph->getAlphabeticBaseline();
            markDirty();
        }
        setMeasuredDimension(static_cast<int>(width), static_cast<int>(height));
        clearDirty();
    }
}

void InputView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (paragraph == nullptr) {
        return;
    }
    auto jniEnv = getContext()->getJniEnv();
    auto jString = (jstring) jniEnv->CallObjectMethod(javaKeyboardRef, getContentMethodId);
    auto str = jniEnv->GetStringUTFChars(jString, nullptr);
    if (content != str) {
        content = str;
        markDirty();
    }
    jniEnv->ReleaseStringUTFChars(jString, str);
    if (isFocused && getContext()->getCurrentTimeMills() % 1000 < 500) {
        if (content.empty()) {
            canvas->drawRect(SkRect::MakeLTRB(skRect.left(),
                                              skRect.bottom() - cursorHeight,
                                              skRect.left() + 10,
                                              skRect.bottom()),
                             *cursorPaint);
        } else {
            auto rect = paragraph->getRectsForPlaceholders()[0].rect;
            canvas->drawRect(SkRect::MakeLTRB(rect.left() + 10 + skRect.left(),
                                              skRect.bottom() - cursorHeight,
                                              rect.right() + skRect.left(),
                                              skRect.bottom()),
                             *cursorPaint);
        }
    }
    paragraph->paint(canvas, skRect.left(), skRect.top());
}

void InputView::setTextSize(int size) {
    this->fontSize = size;
    markDirty();
}

void InputView::setContext(std::shared_ptr<SkiaUIContext> context) {
    View::setContext(context);
    auto jniEnv = getContext()->getJniEnv();
    auto jClazz = jniEnv->FindClass("com/temple/skiaui/keyboard/HYKeyboard");
    auto jConstructor = jniEnv->GetMethodID(jClazz, "<init>",
                                            "(Lcom/temple/skiaui/HYSkiaEngine;J)V");
    auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
    javaKeyboardRef = jniEnv->NewGlobalRef(
            jniEnv->NewObject(jClazz, jConstructor, javaSkiaEngine, reinterpret_cast<long >(this)));
    javaShowMethodId = jniEnv->GetMethodID(jClazz, "show", "(Ljava/lang/String;)V");
    javaHideMethodId = jniEnv->GetMethodID(jClazz, "hide", "()V");
    getContentMethodId = jniEnv->GetMethodID(jClazz, "getContent", "()Ljava/lang/String;");
    releaseMethodId = jniEnv->GetMethodID(jClazz, "release", "()V");
}

void InputView::setFocus(bool focus) {
    this->isFocused = focus;
}

}