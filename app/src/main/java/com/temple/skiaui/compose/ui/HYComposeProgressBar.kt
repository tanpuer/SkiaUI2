package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposeProgressBar(modifier: Modifier) : HYComposeView(modifier) {

    private var onChange: ((progress: Int, finished: Boolean) -> Unit)? = null

    override fun getViewType() = "ProgressBar"

    fun setBarColor(color: Color) {
        nativeSetBarColor(ref, composeColorToSkiaColor(color))
    }

    fun setOnChangeListener(listener: ((progress: Int, finished: Boolean) -> Unit)? = null) {
        onChange = listener
        listener?.let {
            nativeSetOnChangeCallback(ref)
        }
    }

    fun setProgress(progress: Int) {
        nativeSetProgress(ref, progress)
    }

    private fun onChangeFromNative(progress: Int, finished: Boolean) {
        onChange?.invoke(progress, finished)
    }

    private external fun nativeSetBarColor(ref: Long, color: Int)
    private external fun nativeSetOnChangeCallback(ref: Long)
    private external fun nativeSetProgress(ref: Long, progress: Int)

}