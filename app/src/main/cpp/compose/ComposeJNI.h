#pragma once

#include "jni.h"
#include "ComposeNodeJNI.h"
#include "ComposeViewJNI.h"
#include "ComposeVideoJNI.h"
#include "ComposeFlexboxLayoutJNI.h"
#include "ComposePageJNI.h"

static void registerComposeJNI(JNIEnv *jniEnv) {
    RegisterComposeNodeMethods(jniEnv);
    RegisterComposeViewMethods(jniEnv);
    RegisterComposeVideoMethods(jniEnv);
    RegisterComposeFlexboxLayoutMethods(jniEnv);
    RegisterComposePageMethods(jniEnv);
}

static void unRegisterComposeJNI(JNIEnv *jniEnv) {
    UnRegisterComposeNodeMethods(jniEnv);
    UnRegisterComposeViewMethods(jniEnv);
    UnRegisterComposeVideoMethods(jniEnv);
    UnRegisterComposeFlexboxLayoutMethods(jniEnv);
    UnRegisterComposePageMethods(jniEnv);
}