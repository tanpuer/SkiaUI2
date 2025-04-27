package com.temple.skiaui.compose.ui

import android.graphics.Bitmap
import androidx.compose.ui.graphics.Color

interface Canvas {

    fun drawRect(l: Float, t: Float, r: Float, b: Float, paint: HYComposePaint)

    fun drawCircle(cx: Float, cy: Float, radius: Float, paint: HYComposePaint)

    fun drawText(text: String, x: Float, y: Float, textSize: Float, color: Color, width: Int = Int.MAX_VALUE)

    fun drawBitmap(bitmap: Bitmap, x: Float, y: Float, paint: HYComposePaint?)

    fun invalidate()

}