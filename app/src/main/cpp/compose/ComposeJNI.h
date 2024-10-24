#pragma once

#include "jni.h"
#include "ComposeNodeJNI.h"
#include "ComposeViewJNI.h"
#include "ComposeVideoJNI.h"
#include "ComposeFlexboxLayoutJNI.h"

static void registerComposeJNI(JNIEnv *jniEnv) {
    RegisterComposeNodeMethods(jniEnv);
    RegisterComposeViewMethods(jniEnv);
    RegisterComposeVideoMethods(jniEnv);
    RegisterComposeFlexboxLayoutMethods(jniEnv);
}