#pragma once

#include <ITestDraw.h>
#include <Velocity.h>
#include "IFilter.h"
#include "gpu/ganesh/gl/GrGLInterface.h"
#include "gpu/ganesh/GrDirectContext.h"
#include "core/SkSurface.h"
#include "core/SkCanvas.h"

namespace HYSkiaUI {

class TextureSurfaceHelper {
public:
    TextureSurfaceHelper(GrGLuint texID)
            : fTexID(texID) {
        ALOGD("textureId is %d", texID)
    }

    ~TextureSurfaceHelper() {
        ALOGD("textureId is delete %d", fTexID)
        glDeleteTextures(1, &fTexID);
    }

private:
    GrGLuint fTexID;
};

class SkiaFilter : public IFilter {

public:

    SkiaFilter();

    ~SkiaFilter();

    virtual void setWindowSize(int width, int height) override;

    virtual void render(SkPicture *picture) override;

    long MakeHardwareBufferToSkImage(JNIEnv *env, jobject javaHardwareBuffer) override;

    void deleteSkImage(JNIEnv *env, long skImagePtr) override;

    long attachSurfaceTexture(JNIEnv *env, int width, int height, jobject surfaceTexture) override;

private:

    sk_sp<SkSurface> skiaSurface;

    sk_sp<GrDirectContext> skiaContext;

    SkCanvas *skCanvas;

};

}
