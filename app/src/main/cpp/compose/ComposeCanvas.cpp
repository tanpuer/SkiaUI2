#include "ComposeCanvas.h"

namespace HYSkiaUI {

ComposeCanvas::ComposeCanvas() {

}

ComposeCanvas::~ComposeCanvas() {

}

void ComposeCanvas::draw(SkCanvas *canvas) {
    this->canvas = canvas;
    canvas->save();
    canvas->translate(static_cast<float>(left), static_cast<float >(top));
    callComposeDraw(canvas);
    canvas->restore();
}

void ComposeCanvas::setContext(std::shared_ptr<SkiaUIContext> &context) {
    View::setContext(context);
    auto jniEnv = context->getJniEnv();
    auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/ui/HYComposeCanvas");
    onDrawMethodId = jniEnv->GetMethodID(jClazz, "onDrawFromJNI", "()V");
}

void ComposeCanvas::callComposeDraw(SkCanvas *canvas) {
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(globalJavaViewRef, onDrawMethodId);
}

SkCanvas *ComposeCanvas::getCanvas() {
    return canvas;
}

}
