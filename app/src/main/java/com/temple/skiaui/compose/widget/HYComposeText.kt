package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeText(modifier: Modifier) : HYComposeView(modifier) {

    override fun initStyles(modifier: Modifier) {
        super.initStyles(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "textSize" -> {
                    nativeSetTextSize(ref, value as Int)
                }
            }
        }
    }

    fun setText(value: String) {
        nativeSetText(ref, value)
    }

    fun setColor(color: String) {
        nativeSetColor(ref, color)
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
    private external fun nativeSetColor(textView: Long, color: String)
    private external fun nativeSetMaxLine(textView: Long, maxLine: Int)
    private external fun nativeSetEllipsis(textView: Long, ellipse: String)

}