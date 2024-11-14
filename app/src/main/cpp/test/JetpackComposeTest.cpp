#include "JetpackComposeTest.h"

JetpackComposeTest::JetpackComposeTest(JNIEnv *jniEnv) {
    this->jniEnv = jniEnv;
}

JetpackComposeTest::~JetpackComposeTest() {
    jniEnv->DeleteGlobalRef(testRef);
}

void JetpackComposeTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/example/JetpackComposeTest");
        auto constructor = jniEnv->GetMethodID(jClazz, "<init>",
                                               "(Lcom/temple/skiaui/HYSkiaEngine;J)V");
        auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
        auto contextPtr = reinterpret_cast<long>(this);
        testRef = jniEnv->NewGlobalRef(
                jniEnv->NewObject(jClazz, constructor, javaSkiaEngine, contextPtr));
        auto startMethod = jniEnv->GetMethodID(jClazz, "start", "(II)J");
        auto page = reinterpret_cast<Page *>(jniEnv->CallLongMethod(testRef, startMethod, width,
                                                                    height));
        root = page;
        context->getPageStackManager()->push(page);
        page->enterFromRight(Page::EnterExitInfo(width, 0));
    }
    performAnimations(width, height);
    context->getPageStackManager()->removeDestroyedPage();
    for (const auto &item: context->getPageStackManager()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->drawOnFrame(drawCount);
        item->measure();
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}
