package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

class HYComposeEditText(modifier: Modifier) :
    HYComposeView(modifier) {

    fun setHint(hint: String) {
        nativeSetHint(ref, hint)
    }

    fun setFocus(focus: Boolean) {
        nativeSetFocus(ref, focus)
    }

    fun setTextColor(color: Color) {
        if (color == Color.Unspecified) {
            return
        }
        nativeSetTextColor(ref, composeColorToSkiaColor(color))
    }

    fun setHintColor(color: Color) {
        if (color == Color.Unspecified) {
            return
        }
        nativeSetHintColor(ref, composeColorToSkiaColor(color))
    }

    override fun getViewType(): String = "EditText"

    private external fun nativeSetHint(editText: Long, hint: String)
    private external fun nativeSetFocus(editText: Long, focus: Boolean)
    private external fun nativeSetTextColor(editText: Long, color: Int)
    private external fun nativeSetHintColor(editText: Long, color: Int)

}