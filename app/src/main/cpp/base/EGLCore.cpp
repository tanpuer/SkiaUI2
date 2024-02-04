//
// Created by cw on 2021/6/2.
//

#include "EGLCore.h"
#include "native_log.h"

EGLCore::EGLCore() {
    ALOGD("EGLCore constructor")
}

EGLCore::~EGLCore() {
    ALOGD("~EGLCore")
}

int
EGLCore::createGLEnv(EGLContext sharedCtx, ANativeWindow *nativeWindow, int width, int height,
                     bool offScreen) {
    // EGL config attributes
    const EGLint confAttr[] =
            {
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                    EGL_SURFACE_TYPE,
                    EGL_PBUFFER_BIT,//EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
                    EGL_RED_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_BLUE_SIZE, 8,
                    EGL_ALPHA_SIZE, 8,// if you need the alpha channel
                    EGL_DEPTH_SIZE, 16,// if you need the depth buffer
                    EGL_STENCIL_SIZE, 8,
                    EGL_NONE
            };

    // EGL context attributes
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };

    EGLint eglMajVers, eglMinVers;
    EGLint numConfigs;

    int resultCode = 0;
    do {
        //1. 获取 EGLDisplay 对象，建立与本地窗口系统的连接
        mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mEGLDisplay == EGL_NO_DISPLAY) {
            //Unable to open connection to local windowing system
            ALOGD("EGLRender::CreateGlesEnv Unable to open connection to local windowing system")
            resultCode = -1;
            break;
        }

        //2. 初始化 EGL 方法
        if (!eglInitialize(mEGLDisplay, &eglMajVers, &eglMinVers)) {
            // Unable to initialize EGL. Handle and recover
            ALOGE("EGLRender::CreateGlesEnv Unable to initialize EGL")
            resultCode = -1;
            break;
        }

        ALOGD("EGLRender::CreateGlesEnv EGL init with version %d.%d", eglMajVers, eglMinVers)

        //3. 获取 EGLConfig 对象，确定渲染表面的配置信息
        if (!eglChooseConfig(mEGLDisplay, confAttr, &mEGLConf, 1, &numConfigs)) {
            ALOGE("EGLRender::CreateGlesEnv some config is wrong")
            resultCode = -1;
            break;
        }

        //4. 创建渲染表面 EGLSurface, eglCreateWindowSurface; 使用 eglCreatePbufferSurface 创建屏幕外渲染区域
        if (offScreen) {
            int surface_attrib[] = {
                    EGL_WIDTH, width,
                    EGL_HEIGHT, height,
                    EGL_NONE
            };
            mEGLSurface = eglCreatePbufferSurface(mEGLDisplay, mEGLConf, surface_attrib);
            ALOGD("eglCreatePbufferSurface")
        } else {
            int surface_attrib[] = {EGL_NONE};
            mEGLSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConf, nativeWindow,
                                                 surface_attrib);
            ALOGD("eglCreateWindowSurface")
        }
        if (mEGLSurface == EGL_NO_SURFACE) {
            switch (eglGetError()) {
                case EGL_BAD_ALLOC:
                    // Not enough resources available. Handle and recover
                    ALOGE("EGLRender::CreateGlesEnv Not enough resources available")
                    break;
                case EGL_BAD_CONFIG:
                    // Verify that provided EGLConfig is valid
                    ALOGE("EGLRender::CreateGlesEnv provided EGLConfig is invalid")
                    break;
                case EGL_BAD_PARAMETER:
                    // Verify that the EGL_WIDTH and EGL_HEIGHT are
                    // non-negative values
                    ALOGE("EGLRender::CreateGlesEnv provided EGL_WIDTH and EGL_HEIGHT is invalid")
                    break;
                case EGL_BAD_MATCH:
                    // Check window and EGLConfig attributes to determine
                    // compatibility and pbuffer-texture parameters
                    ALOGE("EGLRender::CreateGlesEnv Check window and EGLConfig attributes")
                    break;
            }
        }

        //5. 创建渲染上下文 EGLContext
        mEGLCtx = eglCreateContext(mEGLDisplay, mEGLConf, sharedCtx, ctxAttr);
        if (mEGLCtx == EGL_NO_CONTEXT) {
            EGLint error = eglGetError();
            if (error == EGL_BAD_CONFIG) {
                // Handle error and recover
                ALOGE("EGLRender::CreateGlesEnv EGL_BAD_CONFIG")
                resultCode = -1;
                break;
            }
        }

        //6. 绑定上下文
        if (!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLCtx)) {
            ALOGE("EGLRender::CreateGlesEnv MakeCurrent failed")
            resultCode = -1;
            break;
        }
        ALOGD("EGLRender::CreateGlesEnv initialize success!")
    } while (false);

    if (resultCode != 0) {
        ALOGE("EGLRender::CreateGlesEnv fail")
    }

    return resultCode;
}

void EGLCore::releaseGLEnv() {
    if (mEGLDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(mEGLDisplay, mEGLSurface);
        eglDestroyContext(mEGLDisplay, mEGLCtx);
        eglReleaseThread();
        eglTerminate(mEGLDisplay);
    }
    mEGLDisplay = EGL_NO_DISPLAY;
    mEGLCtx = EGL_NO_CONTEXT;
    mEGLSurface = EGL_NO_SURFACE;
    mEGLConf = nullptr;
    ALOGD("egl release success")
}

EGLSurface EGLCore::getEGLSurface() {
    return mEGLSurface;
}

EGLContext EGLCore::getEGLContext() {
    return mEGLCtx;
}

EGLDisplay EGLCore::getEGLDisplay() {
    return mEGLDisplay;
}

void EGLCore::swapBuffer() {
    if (mEGLDisplay != EGL_NO_DISPLAY && mEGLSurface != EGL_NO_SURFACE) {
        eglSwapBuffers(mEGLDisplay, mEGLSurface);
    }
}

void EGLCore::makeCurrent() {
    eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLCtx);
}
