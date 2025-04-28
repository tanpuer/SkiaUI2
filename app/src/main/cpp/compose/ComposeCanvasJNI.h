#pragma once

#include "jni.h"
#include "native_log.h"
#include "ComposeCanvas.h"
#include "android/bitmap.h"
#include "core/SkColorSpace.h"

using namespace HYSkiaUI;

const char *HYComposeCanvas = "com/temple/skiaui/compose/ui/HYComposeCanvas";

extern "C" JNIEXPORT void JNICALL
compose_canvas_draw_rect(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat l, jfloat t,
                         jfloat r, jfloat b, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawRect(SkRect::MakeLTRB(l, t, r, b), *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_draw_circle(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat cx, jfloat cy,
                           jfloat radius, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawCircle(cx, cy, radius, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_draw_text(JNIEnv *env, jobject instance, jlong canvasPtr, jstring text, jfloat x,
                         jfloat y, float size, int color, int width) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto cString = env->GetStringUTFChars(text, nullptr);
    auto fontCollection = composeCanvas->getContext()->getFontCollection();
    skia::textlayout::ParagraphStyle paraStyle;
    auto paragraphBuilder = ParagraphBuilder::make(paraStyle, fontCollection);
    TextStyle textStyle;
    textStyle.setColor(color);
    textStyle.setFontSize(size);
    std::vector<SkString> fontFamily;
    fontFamily.emplace_back("Alimama");
    textStyle.setFontFamilies(fontFamily);
    paragraphBuilder->pushStyle(textStyle);
    paragraphBuilder->addText(cString);
    auto paragraph = paragraphBuilder->Build();
    paragraph->layout(width);
    paragraph->paint(canvas, x, y);
    env->ReleaseStringUTFChars(text, cString);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_draw_bitmap(JNIEnv *env, jobject instance, jlong canvasPtr, jobject bitmap, jfloat x,
                           jfloat y, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    AndroidBitmapInfo info;
    void *pixels = nullptr;
    auto res = AndroidBitmap_getInfo(env, bitmap, &info);
    if (res != ANDROID_BITMAP_RESULT_SUCCESS) {
        ALOGE("AndroidBitmap_getInfo error %d", res)
        return;
    }
    res = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (res != ANDROID_BITMAP_RESULT_SUCCESS) {
        ALOGE("AndroidBitmap_lockPixels error %d", res)
        return;
    }
    SkColorType colorType = kUnknown_SkColorType;
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        colorType = kRGBA_8888_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        colorType = kRGB_565_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_4444) {
        colorType = kARGB_4444_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_A_8) {
        colorType = kAlpha_8_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_F16) {
        colorType = kRGBA_F16_SkColorType;
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_1010102) {
        colorType = kRGBA_1010102_SkColorType;
    }
    SkImageInfo skInfo = SkImageInfo::Make(
            static_cast<int>(info.width), static_cast<int>(info.height),
            colorType,
            kPremul_SkAlphaType,
            SkColorSpace::MakeSRGB()
    );
    auto skBitmap = std::make_unique<SkBitmap>();
    skBitmap->setInfo(skInfo, info.stride);
    skBitmap->setPixels(pixels);
    AndroidBitmap_unlockPixels(env, bitmap);
    auto skImage = skBitmap->asImage();
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawImage(skImage, x, y, SkSamplingOptions(), paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawPath(JNIEnv *env, jobject instance, jlong canvasPtr, jlong pathPtr,
                        jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto path = reinterpret_cast<SkPath *>(pathPtr);
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawPath(*path, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_invalidate(JNIEnv *env, jobject instance, jlong canvasPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    composeCanvas->markDirty();
}

static JNINativeMethod g_ComposeCanvasMethods[] = {
        {"nativeDrawRect",   "(JFFFFJ)V",                        (void *) compose_canvas_draw_rect},
        {"nativeDrawCircle", "(JFFFJ)V",                         (void *) compose_canvas_draw_circle},
        {"nativeDrawText",   "(JLjava/lang/String;FFFII)V",      (void *) compose_canvas_draw_text},
        {"nativeDrawBitmap", "(JLandroid/graphics/Bitmap;FFJ)V", (void *) compose_canvas_draw_bitmap},
        {"nativeDrawPath",   "(JJJ)V",                           (void *) compose_canvas_drawPath},
        {"nativeInvalidate", "(J)V",                             (void *) compose_canvas_invalidate},
};

static int RegisterComposeCanvasMethods(JNIEnv *env) {
    ALOGD("RegisterComposeCanvasMethods start %s", HYComposeCanvas)
    jclass clazz = env->FindClass(HYComposeCanvas);
    if (clazz == nullptr) {
        ALOGD("RegisterComposeCanvasMethods fail clazz == null")
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_ComposeCanvasMethods, std::size(g_ComposeCanvasMethods)) <
        0) {
        ALOGD("RegisterComposeCanvasMethods fail")
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnRegisterComposeCanvasMethods(JNIEnv *env) {
    jclass clazz = env->FindClass(HYComposeCanvas);
    if (clazz == nullptr) {
        ALOGD("UnRegisterComposeCanvasMethods fail clazz == null")
    }
    env->UnregisterNatives(clazz);
}