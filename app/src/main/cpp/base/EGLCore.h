//
// Created by cw on 2021/6/2.
//

#ifndef HYEDITOR_EGLCORE_H
#define HYEDITOR_EGLCORE_H

#include "EGL/egl.h"
#include "GLES3/gl3.h"
#include "GLES3/gl3ext.h"
#include "EGL/eglext.h"

class EGLCore {

public:

    EGLCore();

    ~EGLCore();

    /**
     * sharedCtx, 共享egl上下文
     * nativeWindow == nullptr 离屏
     * @param nativeWindow
     */
    int createGLEnv(EGLContext sharedCtx, ANativeWindow *nativeWindow, int width, int height, bool offScreen);

    void releaseGLEnv();

    EGLSurface getEGLSurface();

    EGLContext getEGLContext();

    EGLDisplay getEGLDisplay();

    void swapBuffer();

    void makeCurrent();

private:

    EGLConfig mEGLConf;
    EGLSurface mEGLSurface;
    EGLContext mEGLCtx;
    EGLDisplay mEGLDisplay;

};


#endif //HYEDITOR_EGLCORE_H
