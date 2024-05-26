#pragma once

#include <ITestDraw.h>
#include <Velocity.h>
#include "IFilter.h"
#include "gpu/gl/GrGLInterface.h"
#include "gpu/GrDirectContext.h"
#include "core/SkSurface.h"
#include "core/SkCanvas.h"

class SkiaFilter : public IFilter {

public:

    SkiaFilter();

    ~SkiaFilter();

    virtual void setWindowSize(int width, int height) override;

    virtual void render(SkPicture *picture) override;

    long MakeHardwareBufferToSkImage(JNIEnv* env, jobject hardwareBuffer) override;

private:

    sk_sp<SkSurface> skiaSurface;

    sk_sp<GrDirectContext> skiaContext;

    SkCanvas *skCanvas;

};