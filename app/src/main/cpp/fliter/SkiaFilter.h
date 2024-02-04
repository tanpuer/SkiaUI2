//
// Created by cw on 1/21/22.
//

#ifndef SKIAUI_SKIAFILTER_H
#define SKIAUI_SKIAFILTER_H


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

    virtual void doFrame(long time) override;

    void dispatchTouchEvent(TouchEvent *touchEvent) override;

    void setVelocity(Velocity *velocity) override;

private:

    sk_sp<SkSurface> skiaSurface;

    sk_sp<GrDirectContext> skiaContext;

    SkCanvas *skCanvas;

    std::unique_ptr<TouchEvent> mTouchEvent;

    ITestDraw *testDraw;
};


#endif //SKIAUI_SKIAFILTER_H
