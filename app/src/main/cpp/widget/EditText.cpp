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
}

void EditText::clearFocus() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, clearFocusMethodId);
}

void EditText::requestFocus() {
    auto jniEnv = getContext()->getJniEnv();
    jniEnv->CallVoidMethod(javaView, requestFocusMethodId);
}

}
