#pragma once

#include "ITestDraw.h"

namespace HYSkiaUI {

class JetpackComposeTest : public ITestDraw {

public:

    JetpackComposeTest(JNIEnv *jniEnv);

    ~JetpackComposeTest();

    void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

private:

    JNIEnv *jniEnv = nullptr;

    jobject testRef = nullptr;

    bool createFlag = false;

};

}