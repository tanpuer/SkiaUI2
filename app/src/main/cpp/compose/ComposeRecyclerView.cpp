#include "ComposeRecyclerView.h"

namespace HYSkiaUI {

ComposeRecyclerView::ComposeRecyclerView() {

}

ComposeRecyclerView::~ComposeRecyclerView() {

}

View *ComposeRecyclerView::onCreateView(uint32_t index) {
    auto jniEnv = context->getJniEnv();
    auto result = jniEnv->CallLongMethod(globalJavaViewRef, createViewMethodId,
                                         static_cast<jint>(index));
    return reinterpret_cast<View *>(result);
}

void ComposeRecyclerView::onBindView(uint32_t index, HYSkiaUI::View *view) {
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(globalJavaViewRef, bindViewMethodId, static_cast<jint>(index),
                           view->getJavaViewRef());
}

uint32_t ComposeRecyclerView::getViewType(uint32_t index) {
    auto jniEnv = context->getJniEnv();
    auto type = jniEnv->CallIntMethod(globalJavaViewRef, viewTypeMethodId,
                                      static_cast<jint>(index));
    return static_cast<uint32_t>(type);
}

void ComposeRecyclerView::checkJavaViewRef(jobject instance) {
    View::checkJavaViewRef(instance);
    auto jniEnv = context->getJniEnv();
    auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/ui/HYComposeRecyclerView");
    createViewMethodId = jniEnv->GetMethodID(jClazz, "createViewFromJNI", "(I)J");
    bindViewMethodId = jniEnv->GetMethodID(jClazz, "bindViewFromJNI",
                                           "(ILcom/temple/skiaui/compose/ui/HYComposeView;)V");
    viewTypeMethodId = jniEnv->GetMethodID(jClazz, "getViewTypeFromJNI", "(I)I");
}

}
