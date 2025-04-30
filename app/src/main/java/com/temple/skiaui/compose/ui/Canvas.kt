package com.temple.skiaui.compose.ui

import android.graphics.Bitmap
import androidx.compose.ui.graphics.Color

interface Canvas {

    fun save()

    fun restore()

    fun translate(x: Float, y: Float)

    fun scale(sx: Float, sy: Float)

    fun rotate(degrees: Float)

    fun drawRect(l: Float, t: Float, r: Float, b: Float, paint: HYComposePaint)

    fun drawCircle(cx: Float, cy: Float, radius: Float, paint: HYComposePaint)

    fun drawText(
        text: String,
        x: Float,
        y: Float,
        textSize: Float,
        color: Color,
        width: Int = Int.MAX_VALUE
    )

    fun drawBitmap(bitmap: Bitmap, x: Float, y: Float, paint: HYComposePaint?)

    fun drawPath(path: HYComposePath, paint: HYComposePaint)

    fun drawColor(color: Color)

    fun drawPaint(paint: HYComposePaint)

    fun drawPoint(x: Float, y: Float, paint: HYComposePaint)

    fun drawLine(startX: Float, startY: Float, stopX: Float, stopY: Float, paint: HYComposePaint)

    fun drawOval(left: Float, top: Float, right: Float, bottom: Float, paint: HYComposePaint)

    fun drawArc(
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        startAngle: Float,
        sweep: Float,
        useCenter: Boolean,
        paint: HYComposePaint
    )

    fun drawRoundRect(
        left: Float,
        top: Float,
        right: Float,
        bottom: Float,
        rx: Float,
        ry: Float,
        paint: HYComposePaint
    )

    fun drawTextOnPath(
        text: String,
        path: HYComposePath,
        hOffset: Float,
        vOffset: Float,
        flags: Int,
        paint: HYComposePaint
    )

    fun invalidate()

}