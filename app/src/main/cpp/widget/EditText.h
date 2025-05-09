#pragma once

#include "PlatformView.h"

namespace HYSkiaUI {

class EditText : public PlatformView {

public:

    EditText();

    ~EditText();

    const char *getJavaPlatformViewName() override;

    const char *name() override;

    void onJavaViewCreated() override;

    void clearFocus();

    void requestFocus();

    void onHide() override;

    void setHint(const char *hint);

    void setTextColor(int color);

    void setHintColor(int color);

protected:

    jmethodID clearFocusMethodId = nullptr;

    jmethodID requestFocusMethodId = nullptr;

    jmethodID setHintMethodId = nullptr;

    jmethodID setTextColorMethodId = nullptr;

    jmethodID setHintColorMethodId = nullptr;

    std::string hint = "";

    bool focus = false;

};

}
