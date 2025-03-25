#include "EditText.h"

namespace HYSkiaUI {

EditText::EditText() {
}

EditText::~EditText() {
}

const char *EditText::getJavaPlatformViewName() {
    return "com/temple/skiaui/platform/edittext/PlatformEditTextPlugin";
}

const char *EditText::name() {
    return "EditText";
}

void EditText::onJavaViewCreated() {
    PlatformView::onJavaViewCreated();
    auto jniEnv = getContext()->getJniEnv();
    auto javaEditTextPlugin = jniEnv->FindClass(getJavaPlatformViewName());
    clearFocusMethodId = jniEnv->GetMethodID(javaEditTextPlugin, "clearFocus", "()V");
    requestFocusMethodId = jniEnv->GetMethodID(javaEditTextPlugin, "requestFocus", "()V");
    setHintMethodId = jniEnv->GetMethodID(javaEditTextPlugin, "setHint", "(Ljava/lang/String;)V");
    if (!hint.empty()) {
        setHint(hint.c_str());
    }
    if (focus) {
        requestFocus();
    }
}

void EditText::clearFocus() {
    this->focus = false;
    if (clearFocusMethodId == nullptr) {
        return;
    }
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, clearFocusMethodId);
}

void EditText::requestFocus() {
    this->focus = true;
    if (requestFocusMethodId == nullptr) {
        return;
    }
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, requestFocusMethodId);
}

void EditText::onHide() {
    View::onHide();
    clearFocus();
}

void EditText::setHint(const char *hint) {
    this->hint = hint;
    if (setHintMethodId == nullptr) {
        return;
    }
    auto jniEnv = getContext()->getJniEnv();
    auto jString = jniEnv->NewStringUTF(hint);
    jniEnv->CallVoidMethod(javaView, setHintMethodId, jString);
    jniEnv->ReleaseStringUTFChars(jString, hint);
}

}
