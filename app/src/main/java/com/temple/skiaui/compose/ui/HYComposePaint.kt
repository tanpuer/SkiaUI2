package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposePaint {

    var ref: Long = 0

    init {
        ref = nativeInitPaint()
    }

    fun setAntiAlias(aa: Boolean) {
        nativeSetAntiAlias(ref, aa)
    }

    fun setColor(color: Color) {
        nativeSetColor(ref, composeColorToSkiaColor(color))
    }

    private external fun nativeInitPaint(): Long
    private external fun nativeSetAntiAlias(ref: Long, aa: Boolean)
    private external fun nativeSetColor(ref: Long, color: Int)

}