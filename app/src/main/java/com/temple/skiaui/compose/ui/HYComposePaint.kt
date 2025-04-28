package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposePaint : AutoReleasable {

    var ref: Long = nativeInit()

    fun setAntiAlias(aa: Boolean) {
        nativeSetAntiAlias(ref, aa)
    }

    fun setColor(color: Color) {
        nativeSetColor(ref, composeColorToSkiaColor(color))
    }

    override fun release() {
        nativeDeInit(ref)
    }

    private external fun nativeInit(): Long
    private external fun nativeSetAntiAlias(ref: Long, aa: Boolean)
    private external fun nativeSetColor(ref: Long, color: Int)
    private external fun nativeDeInit(ref: Long)

}