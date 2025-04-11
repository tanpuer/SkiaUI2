#pragma once

#include "jni.h"
#include "native_log.h"
#include "CameraView.h"

using namespace HYSkiaUI;

const char *HYComposeCamera = "com/temple/skiaui/compose/ui/HYComposeCamera";

extern "C" JNIEXPORT void JNICALL
compose_camera_capture(JNIEnv *env, jobject instance, jlong viewPtr, jobject callback) {
    auto cameraView = reinterpret_cast<CameraView *>(viewPtr);
    auto imageCallback = env->NewGlobalRef(callback);
    cameraView->capture([env, imageCallback](sk_sp<SkImage> image) {
        jclass callbackClass = env->GetObjectClass(imageCallback);
        jmethodID methodID = env->GetMethodID(callbackClass, "onImageCaptured", "(J)V");
        jlong imagePtr = reinterpret_cast<jlong>(image.get());
        env->CallVoidMethod(imageCallback, methodID, imagePtr);
        env->DeleteGlobalRef(imageCallback);
    });
}

static JNINativeMethod g_ComposeCameraViewMethods[] = {
        {"nativeCapture", "(JLcom/temple/skiaui/compose/ui/CameraCallback;)V",
         (void *) compose_camera_capture},
};

static int RegisterComposeCameraMethods(JNIEnv *env) {
    ALOGD("RegisterComposeCameraMethods start %s", HYComposeCamera)
    jclass clazz = env->FindClass(HYComposeCamera);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeCameraMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeCameraViewMethods,
                             std::size(g_ComposeCameraViewMethods)) < 0) {
        ALOGD("RegisterComposeCameraMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeCameraMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeCamera);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeCameraMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}
