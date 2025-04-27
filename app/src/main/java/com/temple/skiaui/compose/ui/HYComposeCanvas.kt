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

    override fun invalidate() {
        nativeInvalidate(ref)
    }

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

    private external fun nativeInvalidate(
        ref: Long
    )

}