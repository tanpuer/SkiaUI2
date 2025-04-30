#pragma once

#include "jni.h"
#include "native_log.h"
#include "ComposeCanvas.h"
#include "bitmap_util.h"

using namespace HYSkiaUI;

const char *HYComposeCanvas = "com/temple/skiaui/compose/ui/HYComposeCanvas";

extern "C" JNIEXPORT void JNICALL
compose_canvas_save(JNIEnv *env, jobject instance, jlong canvasPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    canvas->save();
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_restore(JNIEnv *env, jobject instance, jlong canvasPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    canvas->restore();
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_translate(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat x, jfloat y) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    canvas->translate(x, y);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_scale(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat sx, jfloat sy) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    canvas->scale(sx, sy);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_rotate(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat degrees) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    canvas->rotate(degrees);
}

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
    textStyle.setColor(static_cast<unsigned int>(color));
    textStyle.setFontSize(size);
    std::vector<SkString> fontFamily;
    fontFamily.emplace_back("Alimama");
    textStyle.setFontFamilies(fontFamily);
    paragraphBuilder->pushStyle(textStyle);
    paragraphBuilder->addText(cString);
    auto paragraph = paragraphBuilder->Build();
    paragraph->layout(static_cast<float >(width));
    paragraph->paint(canvas, x, y);
    env->ReleaseStringUTFChars(text, cString);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_draw_bitmap(JNIEnv *env, jobject instance, jlong canvasPtr, jobject bitmap, jfloat x,
                           jfloat y, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto skImage = transferBitmapToSkImage(env, bitmap);
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
compose_canvas_drawColor(JNIEnv *env, jobject instance, jlong canvasPtr, jint color) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    canvas->drawColor(static_cast<uint32_t>(color));
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawPaint(JNIEnv *env, jobject instance, jlong canvasPtr, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawPaint(*paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_invalidate(JNIEnv *env, jobject instance, jlong canvasPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    composeCanvas->markDirty();
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawPoint(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat x, jfloat y,
                         jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawPoint(x, y, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawLine(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat startX,
                        jfloat startY, jfloat stopX, jfloat stopY, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    canvas->drawLine(startX, startY, stopX, stopY, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawOval(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat left,
                        jfloat top, jfloat right, jfloat bottom, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    SkRect rect{left, top, right, bottom};
    canvas->drawOval(rect, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawArc(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat left,
                       jfloat top, jfloat right, jfloat bottom, jfloat startAngle, jfloat sweep,
                       jboolean useCenter, jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    SkRect rect{left, top, right, bottom};
    canvas->drawArc(rect, startAngle, sweep, useCenter, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawRoundRect(JNIEnv *env, jobject instance, jlong canvasPtr, jfloat left,
                             jfloat top, jfloat right, jfloat bottom, jfloat rx, jfloat ry,
                             jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
    SkRect rect{left, top, right, bottom};
    canvas->drawRoundRect(rect, rx, ry, *paint);
}

extern "C" JNIEXPORT void JNICALL
compose_canvas_drawTextOnPath(JNIEnv *env, jobject instance, jlong canvasPtr, jstring text,
                              jlong pathPtr, jfloat hOffset, jfloat yOffset, jint flags,
                              jlong paintPtr) {
    auto composeCanvas = reinterpret_cast<ComposeCanvas *>(canvasPtr);
    auto canvas = composeCanvas->getCanvas();
    auto path = reinterpret_cast<SkPath *>(pathPtr);
    auto paint = reinterpret_cast<SkPaint *>(paintPtr);
}

static JNINativeMethod g_ComposeCanvasMethods[] = {
        {"nativeSave",           "(J)V",                             (void *) compose_canvas_save},
        {"nativeRestore",        "(J)V",                             (void *) compose_canvas_restore},
        {"nativeTranslate",      "(JFF)V",                           (void *) compose_canvas_translate},
        {"nativeScale",          "(JFF)V",                           (void *) compose_canvas_scale},
        {"nativeRotate",         "(JF)V",                            (void *) compose_canvas_rotate},
        {"nativeDrawRect",       "(JFFFFJ)V",                        (void *) compose_canvas_draw_rect},
        {"nativeDrawCircle",     "(JFFFJ)V",                         (void *) compose_canvas_draw_circle},
        {"nativeDrawText",       "(JLjava/lang/String;FFFII)V",      (void *) compose_canvas_draw_text},
        {"nativeDrawBitmap",     "(JLandroid/graphics/Bitmap;FFJ)V", (void *) compose_canvas_draw_bitmap},
        {"nativeDrawPath",       "(JJJ)V",                           (void *) compose_canvas_drawPath},
        {"nativeInvalidate",     "(J)V",                             (void *) compose_canvas_invalidate},
        {"nativeDrawColor",      "(JI)V",                            (void *) compose_canvas_drawColor},
        {"nativeDrawPaint",      "(JJ)V",                            (void *) compose_canvas_drawPaint},
        {"nativeDrawPoint",      "(JFFJ)V",                          (void *) compose_canvas_drawPoint},
        {"nativeDrawLine",       "(JFFFFJ)V",                        (void *) compose_canvas_drawLine},
        {"nativeDrawOval",       "(JFFFFJ)V",                        (void *) compose_canvas_drawOval},
        {"nativeDrawArc",        "(JFFFFFFZJ)V",                     (void *) compose_canvas_drawArc},
        {"nativeDrawRoundRect",  "(JFFFFFFJ)V",                      (void *) compose_canvas_drawRoundRect},
        {"nativeDrawTextOnPath", "(JLjava/lang/String;JFFIJ)V",      (void *) compose_canvas_drawTextOnPath},
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