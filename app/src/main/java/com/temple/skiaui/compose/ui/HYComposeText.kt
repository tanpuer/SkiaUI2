package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor
import com.temple.skiaui.compose.ui.util.dp2px

open class HYComposeText(modifier: Modifier) : HYComposeView(modifier) {

    override fun initStyles(modifier: Modifier) {
        super.initStyles(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "textSize" -> {
                    nativeSetTextSize(ref, dp2px(value as Dp))
                }
            }
        }
    }

    fun setText(value: String) {
        nativeSetText(ref, value)
    }

    fun setColor(color: Color) {
        nativeSetColor(ref, composeColorToSkiaColor(color))
    }

    fun setMaxLine(maxLine: Int) {
        nativeSetMaxLine(ref, maxLine)
    }

    fun setEllipse(ellipse: String) {
        nativeSetEllipsis(ref, ellipse)
    }

    override fun getViewType(): String = "Text"

    private external fun nativeSetText(textView: Long, text: String)
    private external fun nativeSetTextSize(textView: Long, textSize: Int)
    private external fun nativeSetColor(textView: Long, color: Int)
    private external fun nativeSetMaxLine(textView: Long, maxLine: Int)
    private external fun nativeSetEllipsis(textView: Long, ellipse: String)

}