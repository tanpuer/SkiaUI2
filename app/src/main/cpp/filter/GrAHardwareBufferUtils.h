#pragma once

#include "include/core/SkTypes.h"

#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrTypes.h"

class GrDirectContext;

extern "C" {
typedef struct AHardwareBuffer AHardwareBuffer;
}

typedef void *TexImageCtx;

typedef void (*DeleteImageProc)(TexImageCtx);

typedef void (*UpdateImageProc)(TexImageCtx, GrDirectContext *);

GrBackendTexture
MakeGLBackendTexture(GrDirectContext *dContext, AHardwareBuffer *hardwareBuffer,
                     int width, int height, DeleteImageProc *deleteProc,
                     UpdateImageProc *updateProc, TexImageCtx *imageCtx,
                     bool isProtectedContent,
                     const GrBackendFormat &backendFormat, bool isRenderable);

GrBackendFormat GetGLBackendFormat(GrDirectContext *dContext,
                                   uint32_t bufferFormat,
                                   bool requireKnownFormat);
