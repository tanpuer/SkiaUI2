#include <jni.h>
#include <base/native_log.h>
#include <iterator>
#include "android/native_window_jni.h"
#include "SkiaUIApp.h"

const char *HYSkiaEngine = "com/temple/skiaui/HYSkiaEngine";
jobject globalAssets = nullptr;
static SkiaUIApp *app = nullptr;

extern "C" JNIEXPORT void JNICALL
native_Init(JNIEnv *env, jobject instance, jobject javaAssetManager) {
    ALOGD("native_init")
    globalAssets = env->NewGlobalRef(javaAssetManager);
    app = new SkiaUIApp(env, globalAssets);
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceCreated(JNIEnv *env, jobject instance, jobject javaSurface) {
    ALOGD("native_SurfaceCreated")
    if (app != nullptr) {
        app->create(ANativeWindow_fromSurface(env, javaSurface));
    }
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceChanged(JNIEnv *env, jobject instance, jint width, jint height,
                      jlong time) {
    ALOGD("native_SurfaceChanged")
    if (app != nullptr) {
        app->change(width, height, time);
    }
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceDestroyed(JNIEnv *env, jobject instance) {
    ALOGD("native_SurfaceDestroyed")
    delete app;
    app = nullptr;
}

extern "C" JNIEXPORT void JNICALL
native_SurfaceDoFrame(JNIEnv *env, jobject instance, jlong time) {
    if (app != nullptr) {
        app->doFrame(time);
    }
}

extern "C" JNIEXPORT void JNICALL
native_TouchEvent(JNIEnv *env, jobject instance, jint action, jfloat x, jfloat y) {
    if (app != nullptr) {
        auto touchEvent = new TouchEvent(static_cast<TouchEvent::MotionEvent>(action), x, y);
        app->dispatchTouchEvent(touchEvent);
    }
    ALOGD("native_TouchEvent %d %f %f", action, x, y)
}

extern "C" JNIEXPORT void JNICALL
native_SetVelocity(JNIEnv *env, jobject instance, jfloat x, jfloat y) {
    if (app != nullptr) {
        app->setVelocity(x, y);
    }
    ALOGD("native_SetVelocity %f %f", x, y)
}

static JNINativeMethod g_RenderMethods[] = {
        {"nativeInit",             "(Landroid/content/res/AssetManager;)V", (void *) native_Init},
        {"nativeSurfaceCreated",   "(Landroid/view/Surface;)V",            (void *) native_SurfaceCreated},
        {"nativeSurfaceChanged",   "(IIJ)V",                               (void *) native_SurfaceChanged},
        {"nativeSurfaceDestroyed", "()V",                                  (void *) native_SurfaceDestroyed},
        {"nativeSurfaceDoFrame",   "(J)V",                                 (void *) native_SurfaceDoFrame},
        {"nativeTouchEvent",       "(IFF)Z",                               (void *) native_TouchEvent},
        {"nativeSetVelocity",      "(FF)V",                                (void *) native_SetVelocity}
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