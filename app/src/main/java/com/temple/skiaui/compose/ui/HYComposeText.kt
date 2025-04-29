package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor
import com.temple.skiaui.compose.ui.util.dp2px

open class HYComposeText(modifier: Modifier) : HYComposeView(modifier) {

    fun setTextSize(value: Dp) {
        nativeSetTextSize(ref, dp2px(value as Dp))
    }

    fun setText(value: String) {
        nativeSetText(ref, value)
    }

    fun setColor(color: Color) {
        nativeSetColor(ref, composeColorToSkiaColor(color))
    }

    fun setTextAlign(align: TextAlign) {
        if (align == TextAlign.Unspecified) {
            return
        }
        nativeSetTextAlign(ref, align.value)
    }

    fun setMaxLine(maxLine: Int) {
        nativeSetMaxLine(ref, maxLine)
    }

    fun setEllipse(ellipse: String) {
        nativeSetEllipsis(ref, ellipse)
    }

    fun setFontFamily(fontFamily: String?) {
        if (fontFamily.isNullOrEmpty()) {
            return
        }
        nativeSetFontFamily(ref, fontFamily)
    }

    fun setTextGradient(textGradient: TextGradient?) {
        if (textGradient == null) {
            return
        }
        nativeSetTextGradient(ref, textGradient.colors.map {
            composeColorToSkiaColor(it)
        }.toIntArray(), textGradient.pos)
    }

    override fun getViewType(): String = "Text"

    private external fun nativeSetText(textView: Long, text: String)
    private external fun nativeSetTextSize(textView: Long, textSize: Int)
    private external fun nativeSetColor(textView: Long, color: Int)
    private external fun nativeSetMaxLine(textView: Long, maxLine: Int)
    private external fun nativeSetEllipsis(textView: Long, ellipse: String)
    private external fun nativeSetTextAlign(textView: Long, align: String)
    private external fun nativeSetFontFamily(textView: Long, fontFamily: String)
    private external fun nativeSetTextGradient(textView: Long, colors: IntArray, pos: FloatArray)

}