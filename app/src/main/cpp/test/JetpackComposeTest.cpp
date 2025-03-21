#include "JetpackComposeTest.h"

namespace HYSkiaUI {

JetpackComposeTest::JetpackComposeTest(JNIEnv *jniEnv) {
    this->jniEnv = jniEnv;
}

JetpackComposeTest::~JetpackComposeTest() {
    jniEnv->DeleteGlobalRef(testRef);
}

void JetpackComposeTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (!createFlag) {
        auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/example/HYComposeExampleApp");
        auto constructor = jniEnv->GetMethodID(jClazz, "<init>",
                                               "(Lcom/temple/skiaui/HYSkiaEngine;J)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        auto contextPtr = reinterpret_cast<long>(this);
        testRef = jniEnv->NewGlobalRef(
                jniEnv->NewObject(jClazz, constructor, javaSkiaEngine, contextPtr));
        auto startMethod = jniEnv->GetMethodID(jClazz, "onCreate", "(II)V");
        jniEnv->CallVoidMethod(testRef, startMethod, width,height);
        createFlag = true;
    }
    performAnimations(width, height);
    context->getPageStackManager()->removeDestroyedPage();
    for (const auto &item: context->getPageStackManager()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->drawOneFrame(drawCount);
        item->measure();
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}

}