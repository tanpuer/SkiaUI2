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
    setTextColorMethodId = jniEnv->GetMethodID(javaEditTextPlugin, "setTextColor", "(I)V");
    setHintColorMethodId = jniEnv->GetMethodID(javaEditTextPlugin, "setHintColor", "(I)V");
}

void EditText::clearFocus() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, clearFocusMethodId);
}

void EditText::requestFocus() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, requestFocusMethodId);
}

void EditText::onHide() {
    clearFocus();
}

void EditText::setHint(const char *hint) {
    auto jniEnv = getContext()->getJniEnv();
    auto jString = jniEnv->NewStringUTF(hint);
    jniEnv->CallVoidMethod(javaView, setHintMethodId, jString);
    jniEnv->DeleteLocalRef(jString);
}

void EditText::setTextColor(int color) {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, setTextColorMethodId, color);
}

void EditText::setHintColor(int color) {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, setHintColorMethodId, color);
}

}
