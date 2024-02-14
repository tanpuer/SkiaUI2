#include <jni.h>
#include <base/native_log.h>
#include <iterator>
#include "android/native_window_jni.h"
#include "app/SkiaGLApp.h"
#include "app/SkiaUIApp.h"

const char *HYSkiaEngine = "com/temple/skiaui/HYSkiaEngine";
jobject globalAssets = nullptr;
static SkiaGLApp *glApp = nullptr;
static SkiaUIApp *uiApp = nullptr;

extern "C" JNIEXPORT void JNICALL
native_Init(JNIEnv *env, jobject instance, jobject javaAssetManager) {
    ALOGD("native_init")
    globalAssets = env->NewGlobalRef(javaAssetManager);
    glApp = new SkiaGLApp(env, globalAssets);
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceCreated(JNIEnv *env, jobject instance, jobject javaSurface) {
    ALOGD("native_SurfaceCreated")
    if (glApp != nullptr) {
        glApp->create(ANativeWindow_fromSurface(env, javaSurface));
    }
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceChanged(JNIEnv *env, jobject instance, jint width, jint height, jlong time) {
    ALOGD("native_SurfaceChanged")
    if (glApp != nullptr) {
        glApp->change(width, height, time);
    }
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceDestroyed(JNIEnv *env, jobject instance) {
    ALOGD("native_SurfaceDestroyed")
    if (glApp != nullptr) {
        glApp->destroy();
    }
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceDoFrame(JNIEnv *env, jobject instance, jlong pic, jlong time) {
    if (glApp != nullptr) {
        glApp->doFrame(pic, time);
    }
}

extern "C" JNIEXPORT void JNICALL
native_TouchEvent(JNIEnv *env, jobject instance, jint action, jfloat x, jfloat y) {
    if (uiApp != nullptr) {
        auto touchEvent = new TouchEvent(static_cast<TouchEvent::MotionEvent>(action), x, y);
        uiApp->dispatchTouchEvent(touchEvent);
    }
    ALOGD("native_TouchEvent %d %f %f", action, x, y)
}

extern "C" JNIEXPORT void JNICALL
native_SetVelocity(JNIEnv *env, jobject instance, jfloat x, jfloat y) {
    if (uiApp != nullptr) {
        auto velocity = std::make_unique<Velocity>(x, y);
        uiApp->setVelocity(velocity.get());
    }
    ALOGD("native_SetVelocity %f %f", x, y)
}

extern "C" JNIEXPORT void JNICALL
native_UIInit(JNIEnv *env, jobject instance) {
    uiApp = new SkiaUIApp();
    ALOGD("native_UIInit")
}

extern "C" JNIEXPORT jlong JNICALL
native_UIDoFrame(JNIEnv *env, jobject instance, jlong time) {
    if (uiApp != nullptr) {
        return uiApp->doFrame(time);
    }
    return 0L;
    ALOGD("native_UIDoFrame")
}

extern "C" JNIEXPORT void JNICALL
native_UIChanged(JNIEnv *env, jobject instance, jint width, jint height, jlong time) {
    if (uiApp != nullptr) {
        uiApp->setWindowSize(width, height);
    }
    ALOGD("native_UIChanged")
}

extern "C" JNIEXPORT void JNICALL
native_Release(JNIEnv *env, jobject instance) {
    delete uiApp;
    delete glApp;
    uiApp = nullptr;
    glApp = nullptr;
    ALOGD("native_Release")
}

static JNINativeMethod g_RenderMethods[] = {
        {"nativeInit",             "(Landroid/content/res/AssetManager;)V", (void *) native_Init},
        {"nativeSurfaceCreated",   "(Landroid/view/Surface;)V",             (void *) native_SurfaceCreated},
        {"nativeSurfaceChanged",   "(IIJ)V",                                (void *) native_SurfaceChanged},
        {"nativeSurfaceDestroyed", "()V",                                   (void *) native_SurfaceDestroyed},
        {"nativeSurfaceDoFrame",   "(JJ)V",                                 (void *) native_SurfaceDoFrame},
        {"nativeTouchEvent",       "(IFF)Z",                                (void *) native_TouchEvent},
        {"nativeSetVelocity",      "(FF)V",                                 (void *) native_SetVelocity},
        {"nativeUIInit",           "()V",                                   (void *) native_UIInit},
        {"nativeUIDoFrame",        "(J)J",                                  (void *) native_UIDoFrame},
        {"nativeUIChanged",        "(IIJ)V",                                (void *) native_UIChanged},
        {"nativeRelease",          "()V",                                   (void *) native_Release},
};

static int RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *nativeMethods,
                                 int methodNum) {
    ALOGD("RegisterNativeMethods start %s", className)
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        ALOGD("RegisterNativeMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, nativeMethods, methodNum) < 0) {
        ALOGD("RegisterNativeMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterNativeMethods(JNIEnv *env, const char *className) {
    ALOGD("UnRegisterNativeMethods start")
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        ALOGD("UnRegisterNativeMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}

extern "C" jint JNI_OnLoad(JavaVM *jvm, void *p) {
    JNIEnv *env = nullptr;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    RegisterNativeMethods(env, HYSkiaEngine, g_RenderMethods, std::size(g_RenderMethods));
    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *p) {
    ALOGD("JNI_OnUnload")
    JNIEnv *env = nullptr;
    if (jvm->GetEnv((void **) env, JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    UnRegisterNativeMethods(env, HYSkiaEngine);
}