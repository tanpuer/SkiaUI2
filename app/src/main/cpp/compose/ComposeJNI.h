#pragma once

#include "jni.h"
#include "ComposeNodeJNI.h"
#include "ComposeViewJNI.h"
#include "ComposeVideoJNI.h"
#include "ComposeFlexboxLayoutJNI.h"
#include "ComposePageJNI.h"
#include "ComposeLoadingViewJNI.h"
#include "ComposeLottieJNI.h"
#include "ComposeShaderJNI.h"
#include "ComposeTextJNI.h"
#include "ComposeScrollViewJNI.h"
#include "ComposeImageJNI.h"
#include "ComposeIconJNI.h"
#include "ComposeSwitchJNI.h"
#include "ComposeCameraJNI.h"
#include "ComposeFilamentJNI.h"
#include "ComposeExoVideoJNI.h"
#include "ComposeWebJNI.h"
#include "ComposeSVGJNI.h"

using namespace HYSkiaUI;

static void registerComposeJNI(JNIEnv *jniEnv) {
    RegisterComposeNodeMethods(jniEnv);
    RegisterComposeViewMethods(jniEnv);
    RegisterComposeVideoMethods(jniEnv);
    RegisterComposeFlexboxLayoutMethods(jniEnv);
    RegisterComposePageMethods(jniEnv);
    RegisterComposeLoadingViewMethods(jniEnv);
    RegisterComposeLottieMethods(jniEnv);
    RegisterComposeShaderMethods(jniEnv);
    RegisterComposeTextMethods(jniEnv);
    RegisterComposeScrollMethods(jniEnv);
    RegisterComposeImageMethods(jniEnv);
    RegisterComposeIconMethods(jniEnv);
    RegisterComposeSwitchMethods(jniEnv);
    RegisterComposeCameraMethods(jniEnv);
    RegisterComposeFilamentMethods(jniEnv);
    RegisterComposeExoVideoMethods(jniEnv);
    RegisterComposeWebMethods(jniEnv);
    RegisterComposeSVGMethods(jniEnv);
}

static void unRegisterComposeJNI(JNIEnv *jniEnv) {
    UnRegisterComposeNodeMethods(jniEnv);
    UnRegisterComposeViewMethods(jniEnv);
    UnRegisterComposeVideoMethods(jniEnv);
    UnRegisterComposeFlexboxLayoutMethods(jniEnv);
    UnRegisterComposePageMethods(jniEnv);
    UnRegisterComposeLoadingViewMethods(jniEnv);
    UnRegisterComposeLottieMethods(jniEnv);
    UnRegisterComposeShaderMethods(jniEnv);
    UnRegisterComposeTextMethods(jniEnv);
    UnRegisterComposeScrollMethods(jniEnv);
    UnRegisterComposeImageMethods(jniEnv);
    UnRegisterComposeIconMethods(jniEnv);
    UnRegisterComposeSwitchMethods(jniEnv);
    UnRegisterComposeCameraMethods(jniEnv);
    UnRegisterComposeFilamentMethods(jniEnv);
    UnRegisterComposeExoVideoMethods(jniEnv);
    UnRegisterComposeWebMethods(jniEnv);
    UnRegisterComposeSVGMethods(jniEnv);
}