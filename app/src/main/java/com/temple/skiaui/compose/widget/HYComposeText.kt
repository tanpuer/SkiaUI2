package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeText(modifier: Modifier) : HYComposeView(modifier) {

    override fun initAttrs(modifier: Modifier) {
        super.initAttrs(modifier)
        modifier.attributes.forEach { (key, value) ->
            when (key) {
                "text" -> {
                    nativeSetText(ref, value as String)
                }
            }
        }
    }

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

    override fun getViewType(): String = "Text"

    private external fun nativeSetText(textView: Long, text: String)
    private external fun nativeSetTextSize(textView: Long, textSize: Int)

}