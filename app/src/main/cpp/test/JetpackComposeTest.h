#pragma once

#include "ITestDraw.h"

namespace HYSkiaUI {

class JetpackComposeTest : public ITestDraw {

public:

    JetpackComposeTest(JNIEnv *jniEnv);

    ~JetpackComposeTest();

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    void setContext(std::shared_ptr<SkiaUIContext> &context) override;

private:

    JNIEnv *jniEnv = nullptr;

    jobject testRef = nullptr;

    bool createFlag = false;

    jmethodID composeAppCreateMethodId = nullptr;

    jmethodID composeAppDestroyMethodId = nullptr;

};

}