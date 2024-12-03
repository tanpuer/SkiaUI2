#pragma once

#include <TouchEvent.h>

class SkPicture;

namespace HYSkiaUI {

class IFilter {

public:

    IFilter() {};

    virtual ~IFilter() {};

    virtual void setWindowSize(int width, int height) = 0;

    virtual void render(SkPicture *picture) = 0;

    virtual long MakeHardwareBufferToSkImage(JNIEnv *env, jobject hardwareBuffer) = 0;

    virtual void deleteSkImage(JNIEnv *env, long skImagePtr) = 0;

protected:

    int width = 0;
    int height = 0;

    long time;

};

}