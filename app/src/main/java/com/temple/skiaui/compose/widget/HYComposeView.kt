package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeView(modifier: Modifier) : HYComposeNode(modifier) {

    override fun initStyles(modifier: Modifier) {
        super.initStyles(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "size" -> {
                    val size = value as IntArray
                    setSize(size[0], size[1])
                }

                "backgroundColor" -> {
                    setBackgroundColor(value as String)
                }

                "width" -> {
                    nativeSetWidth(ref, value as Int)
                }

                "height" -> {
                    nativeSetHeight(ref, value as Int)
                }

            }
        }
    }

    fun setSize(width: Int, height: Int) {
        nativeSetWidth(ref, width)
        nativeSetHeight(ref, height)
    }

    fun setBackgroundColor(color: String) {
        nativeSetBackgroundColor(ref, color)
    }

    fun setWidth(width: Int) {
        nativeSetWidth(ref, width)
    }

    fun setHeight(height: Int) {
        nativeSetHeight(ref, height)
    }

    override fun getViewType(): String = "View"

    private external fun nativeSetWidth(view: Long, width: Int)
    private external fun nativeSetHeight(view: Long, height: Int)
    private external fun nativeSetBackgroundColor(view: Long, color: String)
}