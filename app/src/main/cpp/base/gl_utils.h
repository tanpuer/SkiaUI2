//
// Created by Administrator on 2018/2/7.
//

#ifndef CAINCAMERA_GLUTILS_H
#define CAINCAMERA_GLUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

#include "native_log.h"

GLuint createProgram(const char *vertexShader, const char *fragShader);

GLuint loadShader(GLenum type, const char *shaderSrc);

void checkActiveUniform(GLuint program);

GLuint createTexture(GLenum type);

GLuint createTextureWithBytes(unsigned char *bytes, int width, int height);

GLuint createTextureWithOldTexture(GLuint texture, unsigned char *bytes, int width, int height);

void createFrameBuffer(GLuint *framebuffer, GLuint *texture, int width, int height);

void createFrameBuffers(GLuint *frambuffers, GLuint *textures, int width, int height, int size);

void checkGLError(const char *op);

#define SHADER_STRING(s) #s

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

#ifdef __cplusplus
}
#endif

#endif //CAINCAMERA_GLUTILS_H
