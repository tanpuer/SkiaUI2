#pragma once

#include "View.h"

namespace HYSkiaUI {

class InputView : public View {

public:

    InputView();

    ~InputView();

    void setPlaceHolder(const char *placeHolder);

    void setTextSize(int size);

    void measure() override;

    void draw(SkCanvas *canvas) override;

    void setContext(std::shared_ptr<SkiaUIContext>& context) override;

    void setFocus(bool focus);

private:

    void measureText();

    std::unique_ptr<TextStyle> defaultStyle;

    std::unique_ptr<ParagraphBuilder> paragraphBuilder;

    std::unique_ptr<Paragraph> paragraph;

    std::vector<SkString> fontFamily;

    std::unique_ptr<SkPaint> cursorPaint;

    int cursorHeight = 0;

    std::string placeHolder;

    std::string content;

    int fontSize = 100;

    jobject javaKeyboardRef = nullptr;

    jmethodID javaShowMethodId = nullptr;

    jmethodID javaHideMethodId = nullptr;

    jmethodID getContentMethodId = nullptr;

    jmethodID releaseMethodId = nullptr;

    bool isFocused = false;

};

};
