//
// Created by templechen on 2019/3/29.
//

#ifndef VIDEOSHADERDEMO_MATRIX_UTIL_H
#define VIDEOSHADERDEMO_MATRIX_UTIL_H

// -------------------------------------------- matrix部分 -----------------------------------------
#include <GLES3/gl3.h>
#include <cmath>
#include <cstring>
#include <cstdlib>

#define PI 3.1415926f

// 矩阵
typedef struct {
    GLfloat m[16];
} ESMatrix;

/**
 *  缩放
 * @param result
 * @param offset
 * @param sx
 * @param sy
 * @param sz
 */
static void scaleM(ESMatrix *result, int offset, GLfloat sx, GLfloat sy, GLfloat sz) {
    if (result == nullptr) {
        return;
    }
    for (int i = 0; i < 4; ++i) {
        int mi = offset + i;
        result->m[mi] *= sx;
        result->m[mi + 4] *= sy;
        result->m[mi + 8] *= sz;
    }
}

/**
 * 平移
 * @param result
 * @param offset
 * @param x
 * @param y
 * @param z
 */
static void translateM(ESMatrix *result, int offset, GLfloat x, GLfloat y, GLfloat z) {
    for (int i = 0; i < 4; i++) {
        int mi = offset + i;
        result->m[12 + mi] += result->m[mi] * x + result->m[4 + mi] * y + result->m[8 + mi] * z;
    }
}


/**
 * 计算向量长度
 * @param x
 * @param y
 * @param z
 * @return
 */
static float length(float x, float y, float z) {
    return (float) sqrt(x * x + y * y + z * z);
}


/**
 * 设置旋转矩阵
 * @param result
 * @param rmOffset
 * @param a
 * @param x
 * @param y
 * @param z
 */
static void setRotateM(ESMatrix *result, int rmOffset,
                       float a, float x, float y, float z) {
    result->m[rmOffset + 3] = 0;
    result->m[rmOffset + 7] = 0;
    result->m[rmOffset + 11] = 0;
    result->m[rmOffset + 12] = 0;
    result->m[rmOffset + 13] = 0;
    result->m[rmOffset + 14] = 0;
    result->m[rmOffset + 15] = 1;
    a *= (float) (PI / 180.0f);
    float s = (float) sin(a);
    float c = (float) cos(a);
    if (1.0f == x && 0.0f == y && 0.0f == z) {
        result->m[rmOffset + 5] = c;
        result->m[rmOffset + 10] = c;
        result->m[rmOffset + 6] = s;
        result->m[rmOffset + 9] = -s;
        result->m[rmOffset + 1] = 0;
        result->m[rmOffset + 2] = 0;
        result->m[rmOffset + 4] = 0;
        result->m[rmOffset + 8] = 0;
        result->m[rmOffset + 0] = 1;
    } else if (0.0f == x && 1.0f == y && 0.0f == z) {
        result->m[rmOffset + 0] = c;
        result->m[rmOffset + 10] = c;
        result->m[rmOffset + 8] = s;
        result->m[rmOffset + 2] = -s;
        result->m[rmOffset + 1] = 0;
        result->m[rmOffset + 4] = 0;
        result->m[rmOffset + 6] = 0;
        result->m[rmOffset + 9] = 0;
        result->m[rmOffset + 5] = 1;
    } else if (0.0f == x && 0.0f == y && 1.0f == z) {
        result->m[rmOffset + 0] = c;
        result->m[rmOffset + 5] = c;
        result->m[rmOffset + 1] = s;
        result->m[rmOffset + 4] = -s;
        result->m[rmOffset + 2] = 0;
        result->m[rmOffset + 6] = 0;
        result->m[rmOffset + 8] = 0;
        result->m[rmOffset + 9] = 0;
        result->m[rmOffset + 10] = 1;
    } else {
        float len = length(x, y, z);
        if (1.0f != len) {
            float recipLen = 1.0f / len;
            x *= recipLen;
            y *= recipLen;
            z *= recipLen;
        }
        float nc = 1.0f - c;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        result->m[rmOffset + 0] = x * x * nc + c;
        result->m[rmOffset + 4] = xy * nc - zs;
        result->m[rmOffset + 8] = zx * nc + ys;
        result->m[rmOffset + 1] = xy * nc + zs;
        result->m[rmOffset + 5] = y * y * nc + c;
        result->m[rmOffset + 9] = yz * nc - xs;
        result->m[rmOffset + 2] = zx * nc - ys;
        result->m[rmOffset + 6] = yz * nc + xs;
        result->m[rmOffset + 10] = z * z * nc + c;
    }
}

//#define I(_i, _j) ((_j)+ 4*(_i))
#define HYI(_i, _j) ((_j)+ 4*(_i))

/**
 * 矩阵相乘
 * @param result
 * @param lhs
 * @param rhs
 */
static void multiplyMM(ESMatrix *result, ESMatrix *lhs, ESMatrix *rhs) {

    for (int i = 0; i < 4; i++) {
        const float rhs_i0 = rhs->m[HYI(i, 0)];
        float ri0 = lhs->m[HYI(0, 0)] * rhs_i0;
        float ri1 = lhs->m[HYI(0, 1)] * rhs_i0;
        float ri2 = lhs->m[HYI(0, 2)] * rhs_i0;
        float ri3 = lhs->m[HYI(0, 3)] * rhs_i0;
        for (int j = 1; j < 4; j++) {
            const float rhs_ij = rhs->m[HYI(i, j)];
            ri0 += lhs->m[HYI(j, 0)] * rhs_ij;
            ri1 += lhs->m[HYI(j, 1)] * rhs_ij;
            ri2 += lhs->m[HYI(j, 2)] * rhs_ij;
            ri3 += lhs->m[HYI(j, 3)] * rhs_ij;
        }
        result->m[HYI(i, 0)] = ri0;
        result->m[HYI(i, 1)] = ri1;
        result->m[HYI(i, 2)] = ri2;
        result->m[HYI(i, 3)] = ri3;
    }

}

/**
 * 旋转
 * @param result
 * @param angle
 * @param x
 * @param y
 * @param z
 */
static void rotateM(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    // 如果当前的矩阵为空，则重新创建一个
    if (result == nullptr) {
        result = (ESMatrix *) malloc(sizeof(ESMatrix));
        memset(result, 0, sizeof(ESMatrix));
    }
    // 创建临时对象
    auto *temp = (ESMatrix *) malloc(sizeof(ESMatrix));
    memset(temp, 0, sizeof(ESMatrix));
    auto *ret = (ESMatrix *) malloc(sizeof(ESMatrix));
    memset(ret, 0, sizeof(ESMatrix));
    // 设置旋转矩阵
    setRotateM(temp, 0, angle, x, y, z);
    // 矩阵相乘
    multiplyMM(ret, result, temp);
    // 将得到的旋转结果复制给result
    for (int i = 0; i < 16; ++i) {
        result->m[i] = ret->m[i];
    }
    // 释放临时对象
    free(temp);
    free(ret);
}

/**
 * 正交投影矩阵
 * @param result
 * @param mOffset
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 * @return
 */
static int orthoM(ESMatrix *result, int mOffset,
           float left, float right, float bottom, float top,
           float near, float far) {
    if (result == nullptr) {
        return -1;
    }
    if (left == right) {
        return -1;
    }
    if (bottom == top) {
        return -1;
    }
    if (near == far) {
        return -1;
    }

    float r_width = 1.0f / (right - left);
    float r_height = 1.0f / (top - bottom);
    float r_depth = 1.0f / (far - near);
    float x = 2.0f * (r_width);
    float y = 2.0f * (r_height);
    float z = -2.0f * (r_depth);
    float tx = -(right + left) * r_width;
    float ty = -(top + bottom) * r_height;
    float tz = -(far + near) * r_depth;
    result->m[mOffset + 0] = x;
    result->m[mOffset + 5] = y;
    result->m[mOffset + 10] = z;
    result->m[mOffset + 12] = tx;
    result->m[mOffset + 13] = ty;
    result->m[mOffset + 14] = tz;
    result->m[mOffset + 15] = 1.0f;
    result->m[mOffset + 1] = 0.0f;
    result->m[mOffset + 2] = 0.0f;
    result->m[mOffset + 3] = 0.0f;
    result->m[mOffset + 4] = 0.0f;
    result->m[mOffset + 6] = 0.0f;
    result->m[mOffset + 7] = 0.0f;
    result->m[mOffset + 8] = 0.0f;
    result->m[mOffset + 9] = 0.0f;
    result->m[mOffset + 11] = 0.0f;

    return 0;
}


/**
 * 视锥体
 * @param result
 * @param left
 * @param top
 * @param right
 * @param bottom
 * @param nearz
 * @param farz
 */
static int frustumM(ESMatrix *result, int offset, float left, float right,
             float bottom, float top, float near, float far) {
    if (result == nullptr) {
        return -1;
    }
    if (left == right) {
        return -1;
    }
    if (top == bottom) {
        return -1;
    }
    if (near == far) {
        return -1;
    }
    if (near <= 0.0f) {
        return -1;
    }
    if (far <= 0.0f) {
        return -1;
    }

    float r_width = 1.0f / (right - left);
    float r_height = 1.0f / (top - bottom);
    float r_depth = 1.0f / (near - far);
    float x = 2.0f * (near * r_width);
    float y = 2.0f * (near * r_height);
    float A = (right + left) * r_width;
    float B = (top + bottom) * r_height;
    float C = (far + near) * r_depth;
    float D = 2.0f * (far * near * r_depth);
    result->m[offset + 0] = x;
    result->m[offset + 5] = y;
    result->m[offset + 8] = A;
    result->m[offset + 9] = B;
    result->m[offset + 10] = C;
    result->m[offset + 14] = D;
    result->m[offset + 11] = -1.0f;
    result->m[offset + 1] = 0.0f;
    result->m[offset + 2] = 0.0f;
    result->m[offset + 3] = 0.0f;
    result->m[offset + 4] = 0.0f;
    result->m[offset + 6] = 0.0f;
    result->m[offset + 7] = 0.0f;
    result->m[offset + 12] = 0.0f;
    result->m[offset + 13] = 0.0f;
    result->m[offset + 15] = 0.0f;

    return 0;
}

/**
 * 透视矩阵
 * @param result
 * @param offset
 * @param fovy
 * @param aspect
 * @param zNear
 * @param zFar
 * @return
 */
static int perspectiveM(ESMatrix *result, int offset,
                 float fovy, float aspect, float zNear, float zFar) {
    if (result == nullptr) {
        return -1;
    }

    float f = 1.0f / tanf((float) (fovy * (PI / 360.0)));
    float rangeReciprocal = 1.0f / (zNear - zFar);

    result->m[offset + 0] = f / aspect;
    result->m[offset + 1] = 0.0f;
    result->m[offset + 2] = 0.0f;
    result->m[offset + 3] = 0.0f;

    result->m[offset + 4] = 0.0f;
    result->m[offset + 5] = f;
    result->m[offset + 6] = 0.0f;
    result->m[offset + 7] = 0.0f;

    result->m[offset + 8] = 0.0f;
    result->m[offset + 9] = 0.0f;
    result->m[offset + 10] = (zFar + zNear) * rangeReciprocal;
    result->m[offset + 11] = -1.0f;

    result->m[offset + 12] = 0.0f;
    result->m[offset + 13] = 0.0f;
    result->m[offset + 14] = 2.0f * zFar * zNear * rangeReciprocal;
    result->m[offset + 15] = 0.0f;

    return 0;
}

/**
 * 设置视图矩阵
 * @param result
 * @param rmOffset
 * @param eyeX
 * @param eyeY
 * @param eyeZ
 * @param centerX
 * @param centerY
 * @param centerZ
 * @param upX
 * @param upY
 * @param upZ
 * @return
 */
static int setLookAtM(ESMatrix *result, int rmOffset,
               float eyeX, float eyeY, float eyeZ,
               float centerX, float centerY, float centerZ, float upX, float upY,
               float upZ) {

    if (result == nullptr) {
        return -1;
    }

    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    // Normalize f
    float rlf = 1.0f / length(fx, fy, fz);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    // compute s = f x up (x means "cross product")
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;

    // and normalize s
    float rls = 1.0f / length(sx, sy, sz);
    sx *= rls;
    sy *= rls;
    sz *= rls;

    // compute u = s x f
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;


    result->m[rmOffset + 0] = sx;
    result->m[rmOffset + 1] = ux;
    result->m[rmOffset + 2] = -fx;
    result->m[rmOffset + 3] = 0.0f;

    result->m[rmOffset + 4] = sy;
    result->m[rmOffset + 5] = uy;
    result->m[rmOffset + 6] = -fy;
    result->m[rmOffset + 7] = 0.0f;

    result->m[rmOffset + 8] = sz;
    result->m[rmOffset + 9] = uz;
    result->m[rmOffset + 10] = -fz;
    result->m[rmOffset + 11] = 0.0f;

    result->m[rmOffset + 12] = 0.0f;
    result->m[rmOffset + 13] = 0.0f;
    result->m[rmOffset + 14] = 0.0f;
    result->m[rmOffset + 15] = 1.0f;

    translateM(result, rmOffset, -eyeX, -eyeY, -eyeZ);

    return 0;
}

/**
 * 产生一个单位矩阵
 * @param result
 */
static void setIdentityM(ESMatrix *result) {
    if (result == nullptr) {
        result = (ESMatrix *) malloc(sizeof(ESMatrix));
    }
    memset(result, 0x0, sizeof(ESMatrix));
    for (int i = 0; i < 16; i += 5) {
        result->m[i] = 1.0f;
    }
}

#endif //VIDEOSHADERDEMO_MATRIX_UTIL_H
