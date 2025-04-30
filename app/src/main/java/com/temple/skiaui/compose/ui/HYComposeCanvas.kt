package com.temple.skiaui.compose.ui

import android.graphics.Bitmap
import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposeCanvas(modifier: Modifier) : HYComposeView(modifier), Canvas {

    private var onDrawCallback: (canvas: Canvas) -> Unit = {}

    override fun getViewType(): String = "Canvas"

    private fun onDrawFromJNI() {
        onDrawCallback.invoke(this)
    }

    fun setDrawCallback(callback: (canvas: Canvas) -> Unit) {
        onDrawCallback = callback
    }

    override fun save() {
        nativeSave(ref)
    }

    override fun restore() {
        nativeRestore(ref)
    }

    override fun translate(x: Float, y: Float) {
        nativeTranslate(ref, x, y)
    }

    override fun scale(sx: Float, sy: Float) {
        nativeScale(ref, sx, sy)
    }

    override fun rotate(degrees: Float) {
        nativeRotate(ref, degrees)
    }

    override fun drawRect(l: Float, t: Float, r: Float, b: Float, paint: HYComposePaint) {
        nativeDrawRect(ref, l, t, r, b, paint.ref)
    }

    override fun drawCircle(cx: Float, cy: Float, radius: Float, paint: HYComposePaint) {
        nativeDrawCircle(ref, cx, cy, radius, paint.ref)
    }

    override fun drawText(
        text: String,
        x: Float,
        y: Float,
        textSize: Float,
        color: Color,
        width: Int
    ) {
        nativeDrawText(ref, text, x, y, textSize, composeColorToSkiaColor(color), width)
    }

    override fun drawBitmap(bitmap: Bitmap, x: Float, y: Float, paint: HYComposePaint?) {
        nativeDrawBitmap(ref, bitmap, x, y, paint?.ref ?: 0L)
    }

    override fun drawPath(path: HYComposePath, paint: HYComposePaint) {
        nativeDrawPath(ref, path.ref, paint.ref)
    }

    override fun drawColor(color: Color) {
        nativeDrawColor(ref, composeColorToSkiaColor(color))
    }

    override fun drawPaint(paint: HYComposePaint) {
        nativeDrawPaint(ref, paint.ref)
    }

    override fun drawPoint(x: Float, y: Float, paint: HYComposePaint) {
        nativeDrawPoint(ref, x, y, paint.ref)
    }

    override fun drawLine(
        startX: Float,
        startY: Float,
        stopX: Float,
        stopY: Float,
        paint: HYComposePaint
    ) {
        nativeDrawLine(ref, startX, startY, stopX, stopY, paint.ref)
    }

    override fun drawOval(
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        paint: HYComposePaint
    ) {
        nativeDrawOval(ref, left, top, right, bottom, paint.ref)
    }

    override fun drawArc(
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        startAngle: Float,
        sweep: Float,
        useCenter: Boolean,
        paint: HYComposePaint
    ) {
        nativeDrawArc(ref, left, top, right, bottom, startAngle, sweep, useCenter, paint.ref)
    }

    override fun drawRoundRect(
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        rx: Float,
        ry: Float,
        paint: HYComposePaint
    ) {
        nativeDrawRoundRect(ref, left, top, right, bottom, rx, ry, paint.ref)
    }

    override fun drawTextOnPath(
        text: String,
        path: HYComposePath,
        hOffset: Float,
        vOffset: Float,
        flags: Int,
        paint: HYComposePaint
    ) {
        nativeDrawTextOnPath(ref, text, path.ref, hOffset, vOffset, flags, path.ref)
    }

    override fun invalidate() {
        nativeInvalidate(ref)
    }

    //----------------------------------------------native------------------------------------------

    private external fun nativeSave(ref: Long)
    private external fun nativeRestore(ref: Long)
    private external fun nativeTranslate(ref: Long, x: Float, y: Float)
    private external fun nativeScale(ref: Long, sx: Float, sy: Float)
    private external fun nativeRotate(ref: Long, degrees: Float)
    private external fun nativeDrawRect(
        ref: Long,
        l: Float,
        t: Float,
        r: Float,
        b: Float,
        paint: Long
    )

    private external fun nativeDrawCircle(
        ref: Long,
        cx: Float,
        cy: Float,
        radius: Float,
        paint: Long
    )

    private external fun nativeDrawText(
        ref: Long,
        text: String,
        x: Float,
        y: Float,
        textSize: Float,
        color: Int,
        width: Int
    )

    private external fun nativeDrawBitmap(
        ref: Long,
        bitmap: Bitmap,
        x: Float,
        y: Float,
        paint: Long
    )

    private external fun nativeDrawPath(ref: Long, path: Long, paint: Long)
    private external fun nativeDrawColor(ref: Long, color: Int)
    private external fun nativeDrawPaint(ref: Long, paint: Long)
    private external fun nativeDrawPoint(ref: Long, x: Float, y: Float, paint: Long)
    private external fun nativeDrawLine(
        ref: Long,
        startX: Float,
        startY: Float,
        stopX: Float,
        stopY: Float,
        paint: Long
    )

    private external fun nativeDrawOval(
        ref: Long,
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        paint: Long
    )

    private external fun nativeDrawArc(
        ref: Long,
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        startAngle: Float,
        sweep: Float,
        useCenter: Boolean,
        paint: Long
    )

    private external fun nativeDrawRoundRect(
        ref: Long,
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        rx: Float,
        ry: Float,
        paint: Long
    )

    private external fun nativeDrawTextOnPath(
        ref: Long,
        text: String,
        path: Long,
        hOffset: Float,
        vOffset: Float,
        flags: Int,
        paint: Long
    )

    private external fun nativeInvalidate(
        ref: Long
    )

}