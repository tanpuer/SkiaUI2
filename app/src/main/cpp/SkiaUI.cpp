#include <jni.h>
#include <base/native_log.h>
#include <iterator>
#include "android/native_window_jni.h"
#include "app/SkiaGLApp.h"
#include "app/SkiaUIApp.h"
#include "PluginManager.h"
#include "compose/ComposeJNI.h"
#include "WebView.h"
#include "inspect/WebSocketServer.h"
#include "bitmap/AndroidBitmap.h"

using namespace HYSkiaUI;

const char *HYSkiaEngine = "com/temple/skiaui/HYSkiaEngine";

extern "C" JNIEXPORT jlong JNICALL
native_GLInit(JNIEnv *env, jobject instance) {
    ALOGD("native_init")
    auto glApp = new SkiaGLApp(env);
    return reinterpret_cast<long>(glApp);
}

extern "C" JNIEXPORT void JNICALL
native_GLCreated(JNIEnv *env, jobject instance, jlong javaGLApp, jobject javaSurface) {
    ALOGD("native_SurfaceCreated")
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        glApp->create(ANativeWindow_fromSurface(env, javaSurface));
    }
}

extern "C" JNIEXPORT void JNICALL
native_GLChanged(JNIEnv *env, jobject instance, jlong javaGLApp, jint width, jint height,
                 jlong time) {
    ALOGD("native_SurfaceChanged")
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        glApp->change(width, height, time);
    }
}

extern "C" JNIEXPORT void JNICALL
native_GLDestroyed(JNIEnv *env, jobject instance, jlong javaGLApp) {
    ALOGD("native_SurfaceDestroyed")
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        glApp->destroy();
    }
}

extern "C" JNIEXPORT void JNICALL
native_GLDoFrame(JNIEnv *env, jobject instance, jlong javaGLApp, jlong pic, jlong time) {
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        glApp->doFrame(pic, time);
    }
}

extern "C" JNIEXPORT jlong JNICALL
native_GLMakeHardwareBufferToSkImage(JNIEnv *env, jobject instance, jlong javaGLApp,
                                     jobject hardwareBuffer) {
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        return glApp->MakeHardwareBufferToSkImage(env, hardwareBuffer);
    }
    return 0L;
}

extern "C" JNIEXPORT void JNICALL
native_DeleteSkImage(JNIEnv *env, jobject instance, jlong javaGLApp, jlong skImagePtr) {
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        return glApp->deleteSkImage(env, skImagePtr);
    }
}

extern "C" JNIEXPORT void JNICALL
native_TouchEvent(JNIEnv *env, jobject instance, jlong javaUIApp, jint action, jfloat x, jfloat y) {
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        auto touchEvent = new TouchEvent(static_cast<TouchEvent::MotionEvent>(action), x, y);
        uiApp->dispatchTouchEvent(touchEvent);
    }
    ALOGD("native_TouchEvent %d %f %f", action, x, y)
}

extern "C" JNIEXPORT void JNICALL
native_SetVelocity(JNIEnv *env, jobject instance, jlong javaUIApp, jfloat x, jfloat y) {
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        auto velocity = std::make_unique<Velocity>(x, y);
        uiApp->setVelocity(velocity.get());
    }
    ALOGD("native_SetVelocity %f %f", x, y)
}

extern "C" JNIEXPORT jlong JNICALL
native_UIInit(JNIEnv *env, jobject instance, jobject javaAssetManager, jint exampleType) {
    auto globalAssets = env->NewGlobalRef(javaAssetManager);
    auto uiApp = new SkiaUIApp(env, globalAssets, instance, exampleType);
    ALOGD("native_UIInit")
    return reinterpret_cast<long>(uiApp);
}

extern "C" JNIEXPORT jlong JNICALL
native_UIDoFrame(JNIEnv *env, jobject instance, jlong javaUIApp, jlong time) {
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        return uiApp->doFrame(time);
    }
    return 0L;
    ALOGD("native_UIDoFrame")
}

extern "C" JNIEXPORT void JNICALL
native_UIChanged(JNIEnv *env, jobject instance, jlong javaUIApp, jint width, jint height,
                 jlong time) {
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->setWindowSize(width, height);
    }
    ALOGD("native_UIChanged")
}

extern "C" JNIEXPORT void JNICALL
native_Release(JNIEnv *env, jobject instance, jlong javaUIApp, jlong javaGLApp) {
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    uiApp->releaseJavaPluginManager(env);
    delete uiApp;
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    delete glApp;
    ALOGD("native_Release")
}

extern "C" JNIEXPORT jboolean JNICALL
native_BackPressed(JNIEnv *env, jobject instance, jlong javaUIApp) {
    ALOGD("native_BackPressed")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        return uiApp->onBackPressed();
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL
native_SetPlugins(JNIEnv *env, jobject instance, jlong javaUIApp, jobject javaPlugins) {
    ALOGD("native_BackPressed")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->initJavaPluginManager(env, javaPlugins);
    }
    return false;
}

extern "C" JNIEXPORT void JNICALL
native_ExecuteTask(JNIEnv *env, jobject instance, jlong javaUIApp, jint taskId,
                   jobject javaAssets) {
    ALOGD("native_ExecuteTask: %d", taskId)
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->executeTask(env, taskId, javaAssets);
    }
}

extern "C" JNIEXPORT void JNICALL
native_PostTask(JNIEnv *env, jobject instance, jlong javaUIApp, jint taskId) {
    ALOGD("native_PostTask: %d", taskId)
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->postTask(env, taskId);
    }
}

extern "C" JNIEXPORT void JNICALL
native_UIShow(JNIEnv *env, jobject instance, jlong javaUIApp) {
    ALOGD("native_UIShow")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->onShow();
    }
}

extern "C" JNIEXPORT void JNICALL
native_UIHide(JNIEnv *env, jobject instance, jlong javaUIApp) {
    ALOGD("native_UIShow")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->onHide();
    }
}

extern "C" JNIEXPORT void JNICALL
native_DeleteSkPicture(JNIEnv *env, jobject instance, jlong javaUIApp, jlong skPicture) {
//    ALOGD("native_DeleteSkPicture")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->deleteSkPicture(skPicture);
    }
}

extern "C" JNIEXPORT void JNICALL
native_RegisterJetpackCompose(JNIEnv *env, jobject instance) {
    ALOGD("native_RegisterJetpackCompose")
    registerComposeJNI(env);
}

extern "C" JNIEXPORT void JNICALL
native_PerformTimeout(JNIEnv *env, jobject instance, jlong javaUIApp, jlong id) {
//    ALOGD("native_PerformTimeout")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp != nullptr) {
        uiApp->performTimeout(id);
    }
}

extern "C" JNIEXPORT void JNICALL
native_WebViewProgressChange(JNIEnv *env, jobject instance, jlong webViewPtr, jint progress) {
    ALOGD("native_WebViewProgressChange")
    auto webView = reinterpret_cast<WebView *>(webViewPtr);
    if (webView != nullptr) {
        webView->setProgress(progress);
    }
}

extern "C" JNIEXPORT jlong JNICALL
native_AttachSurfaceTexture(JNIEnv *env, jobject instance, jlong javaGLApp, jint width, jint height,
                            jobject surfaceTexture) {
    ALOGD("native_AttachSurfaceTexture")
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        return glApp->attachSurfaceTexture(env, width, height, surfaceTexture);
    }
    return 0L;
}

extern "C" JNIEXPORT void JNICALL
native_UpdateTexImage(JNIEnv *env, jobject instance, jlong javaGLApp,
                      jobject surfaceTexture, jlong skImagePtr) {
//    ALOGD("native_UpdateTexImage")
    auto glApp = reinterpret_cast<SkiaGLApp *>(javaGLApp);
    if (glApp != nullptr) {
        return glApp->updateTexImage(env, surfaceTexture, skImagePtr);
    }
}

extern "C" JNIEXPORT void JNICALL
native_MarkDirty(JNIEnv *env, jobject instance, jobject viewPtr) {
//    ALOGD("native_MarkDirty")
    auto view = reinterpret_cast<PlatformView *>(viewPtr);
    if (view != nullptr) {
        view->markDirty();
    }
}

extern "C" JNIEXPORT void JNICALL
native_SendInspectMsg(JNIEnv *env, jobject instance, jstring message, jobject ptr) {
    ALOGD("native_SendInspectMsg")
    auto server = reinterpret_cast<WebSocketServer *>(ptr);
    if (server != nullptr) {
        auto res = env->GetStringUTFChars(message, nullptr);
        server->receiveMessage(res);
        env->ReleaseStringUTFChars(message, res);
    }
}

extern "C" JNIEXPORT void JNICALL
native_UpdateAndroidBitmap(JNIEnv *env, jobject instance, jlong javaUIApp, jlong ref,
                           jobject bitmap, jint index, jint frameCount) {
//    ALOGD("native_UpdateAndroidBitmap")
    auto uiApp = reinterpret_cast<SkiaUIApp *>(javaUIApp);
    if (uiApp == nullptr) {
        return;
    }
    auto androidBitmap = reinterpret_cast<AndroidBitmap *>(ref);
    if (androidBitmap != nullptr) {
        androidBitmap->setJavaBitmap(env, bitmap, index, frameCount);
    }
}

static JNINativeMethod g_RenderMethods[] = {
        {"nativeGLInit",                        "()J",                                          (void *) native_GLInit},
        {"nativeGLCreated",                     "(JLandroid/view/Surface;)V",                   (void *) native_GLCreated},
        {"nativeGLChanged",                     "(JIIJ)V",                                      (void *) native_GLChanged},
        {"nativeGLDestroyed",                   "(J)V",                                         (void *) native_GLDestroyed},
        {"nativeGLDoFrame",                     "(JJJ)V",                                       (void *) native_GLDoFrame},
        {"nativeGLMakeHardwareBufferToSkImage", "(JLandroid/hardware/HardwareBuffer;)J",        (void *) native_GLMakeHardwareBufferToSkImage},
        {"nativeDeleteSkImage",                 "(JJ)V",                                        (void *) native_DeleteSkImage},
        {"nativeTouchEvent",                    "(JIFF)Z",                                      (void *) native_TouchEvent},
        {"nativeSetVelocity",                   "(JFF)V",                                       (void *) native_SetVelocity},
        {"nativeUIInit",                        "(Landroid/content/res/AssetManager;I)J",       (void *) native_UIInit},
        {"nativeUIDoFrame",                     "(JJ)J",                                        (void *) native_UIDoFrame},
        {"nativeUIChanged",                     "(JIIJ)V",                                      (void *) native_UIChanged},
        {"nativeRelease",                       "(JJ)V",                                        (void *) native_Release},
        {"nativeBackPressed",                   "(J)Z",                                         (void *) native_BackPressed},
        {"nativeSetPlugins",                    "(JLcom/temple/skiaui/plugin/PluginManager;)V", (void *) native_SetPlugins},
        {"nativeExecuteTask",                   "(JILandroid/content/res/AssetManager;)V",      (void *) native_ExecuteTask},
        {"nativePostTask",                      "(JI)V",                                        (void *) native_PostTask},
        {"nativeUIShow",                        "(J)V",                                         (void *) native_UIShow},
        {"nativeUIHide",                        "(J)V",                                         (void *) native_UIHide},
        {"nativeDeleteSkPicture",               "(JJ)V",                                        (void *) native_DeleteSkPicture},
        {"nativeRegisterJetpackCompose",        "()V",                                          (void *) native_RegisterJetpackCompose},
        {"nativePerformTimeout",                "(JJ)V",                                        (void *) native_PerformTimeout},
        {"nativeWebViewProgressChange",         "(JI)V",                                        (void *) native_WebViewProgressChange},
        {"nativeAttachSurfaceTexture",          "(JIILandroid/graphics/SurfaceTexture;)J",      (void *) native_AttachSurfaceTexture},
        {"nativeUpdateTexImage",                "(JLandroid/graphics/SurfaceTexture;J)V",       (void *) native_UpdateTexImage},
        {"nativeMarkDirty",                     "(J)V",                                         (void *) native_MarkDirty},
        {"nativeSendInspectMsg",                "(Ljava/lang/String;J)V",                       (void *) native_SendInspectMsg},
        {"nativeUpdateAndroidBitmap",           "(JJLandroid/graphics/Bitmap;II)V",             (void *) native_UpdateAndroidBitmap},
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