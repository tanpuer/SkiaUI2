package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposeLoadingView(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Loading"

    fun setColor(color: Color) {
        nativeSetColor(ref, composeColorToSkiaColor(color))
    }

    private external fun nativeSetColor(loadingView: Long, color: Int)

}