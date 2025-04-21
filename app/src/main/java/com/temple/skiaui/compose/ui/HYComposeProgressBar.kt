package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposeProgressBar(modifier: Modifier) : HYComposeView(modifier) {

    private var onChange: ((progress: Int) -> Unit)? = null

    override fun getViewType() = "ProgressBar"

    fun setBarColor(color: Color) {
        nativeSetBarColor(ref, composeColorToSkiaColor(color))
    }

    fun setOnChangeListener(listener: ((progress: Int) -> Unit)? = null) {
        onChange = listener
        listener?.let {
            nativeSetOnChangeCallback(ref)
        }
    }

    private fun onChangeFromNative(progress: Int) {
        onChange?.invoke(progress)
    }

    private external fun nativeSetBarColor(ref: Long, color: Int)
    private external fun nativeSetOnChangeCallback(ref: Long)

}