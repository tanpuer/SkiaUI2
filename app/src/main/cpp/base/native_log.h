//
// Created by templechen on 2019/2/24.
//

#ifndef AUDIOEFFECTDEMO_NATIVE_LOG_H
#define AUDIOEFFECTDEMO_NATIVE_LOG_H

#include <android/log.h>

#define Android 1
#define JNI_DEBUG 1
#define JNI_TAG "SkiaUI"

#ifdef Android
#define ALOGE(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_ERROR, JNI_TAG, format, ##__VA_ARGS__); }
#define ALOGI(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_INFO,  JNI_TAG, format, ##__VA_ARGS__); }
#define ALOGD(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG, format, ##__VA_ARGS__); }
#define ALOGW(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_WARN,  JNI_TAG, format, ##__VA_ARGS__); }
#else

#endif

#endif //AUDIOEFFECTDEMO_NATIVE_LOG_H
