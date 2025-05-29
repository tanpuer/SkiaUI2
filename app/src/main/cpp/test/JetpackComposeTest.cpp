#include "JetpackComposeTest.h"
#include "compose/ComposeContext.h"

namespace HYSkiaUI {

JetpackComposeTest::JetpackComposeTest(JNIEnv *jniEnv) {
    this->jniEnv = jniEnv;
}

void JetpackComposeTest::setContext(std::shared_ptr<SkiaUIContext> &context) {
    ITestDraw::setContext(context);
    ComposeContext::getInstance()->saveContext(context);
}

JetpackComposeTest::~JetpackComposeTest() {
    context->callComposeSDKPoped();
    jniEnv->CallVoidMethod(testRef, composeAppDestroyMethodId);
    ComposeContext::getInstance()->clearContext();
    jniEnv->DeleteGlobalRef(testRef);
    auto page = context->getPageStackManager()->pop();
    delete page;
}

void JetpackComposeTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (!createFlag) {
        MeasureTime measureTime("HYComposeExampleApp new");
        auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/example/HYComposeExampleApp");
        auto constructor = jniEnv->GetMethodID(jClazz, "<init>",
                                               "(Lcom/temple/skiaui/HYSkiaEngine;)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        testRef = jniEnv->NewGlobalRef(
                jniEnv->NewObject(jClazz, constructor, javaSkiaEngine));
        composeAppCreateMethodId = jniEnv->GetMethodID(jClazz, "innerOnCreate", "(II)V");
        composeAppDestroyMethodId = jniEnv->GetMethodID(jClazz, "innerOnDestroy", "()V");
        jniEnv->CallVoidMethod(testRef, composeAppCreateMethodId, width, height);
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