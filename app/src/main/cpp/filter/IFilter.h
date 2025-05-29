#pragma once

#include <TouchEvent.h>

class SkPicture;

namespace HYSkiaUI {

class IFilter {

public:

    IFilter() = default;

    virtual ~IFilter() = default;

    virtual void setWindowSize(int width, int height) = 0;

    virtual void render(SkPicture *picture) = 0;

    virtual long MakeHardwareBufferToSkImage(JNIEnv *env, jobject hardwareBuffer) = 0;

    virtual void deleteSkImage(JNIEnv *env, long skImagePtr) = 0;

    virtual long attachSurfaceTexture(JNIEnv* env, int width, int height, jobject surfaceTexture) = 0;

    virtual void updateTexImage(JNIEnv* env, jobject surfaceTexture, long skImagePtr) = 0;

protected:

    int width = 0;
    int height = 0;

    long time = 0L;

};

}