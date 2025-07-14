#include "ComposeTextureView.h"

namespace HYSkiaUI {

ComposeTextureView::ComposeTextureView() {

}

ComposeTextureView::~ComposeTextureView() {
    javaInstance = nullptr;
    //javaInstance = globalJavaViewRef; globalJavaViewRef will be released in View.cpp
}

void ComposeTextureView::setContext(std::shared_ptr<SkiaUIContext> &context) {
    View::setContext(context);
    auto jniEnv = context->getJniEnv();
    javaClass = jniEnv->FindClass(getJavaClassPath());
    getSkImageMethodId = jniEnv->GetMethodID(javaClass, "getSkImage", "()J");
    showMethod = jniEnv->GetMethodID(javaClass, "onShow", "()V");
    hideMethod = jniEnv->GetMethodID(javaClass, "onHide", "()V");
    releaseMethod = jniEnv->GetMethodID(javaClass, "release", "()V");
    sendTouchEventMethodId = jniEnv->GetMethodID(javaClass, "sendTouchEvent", "(IFF)V");
    onSizeChangeMethodId = jniEnv->GetMethodID(javaClass, "onSizeChange", "(II)V");
}

const char *ComposeTextureView::getJavaClassPath() {
    return "com/temple/skiaui/compose/ui/HYComposeTextureView";
}

void ComposeTextureView::checkJavaViewRef(jobject instance) {
    View::checkJavaViewRef(instance);
    javaInstance = globalJavaViewRef;
}

}
