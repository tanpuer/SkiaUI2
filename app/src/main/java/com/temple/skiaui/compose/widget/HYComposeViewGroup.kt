package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeViewGroup(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "ViewGroup"

    override fun initStyles(modifier: Modifier) {
        super.initStyles(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "flexDirection" -> {
                    nativeSetFlexDirection(ref, value as String)
                }

                "justifyContent" -> {
                    nativeSetJustifyContent(ref, value as String)
                }

                "alignItems" -> {
                    nativeSetAlignItems(ref, value as String)
                }
            }
        }
    }

    fun setFlexDirection(value: String) {
        nativeSetFlexDirection(ref, value)
    }

    fun setJustifyContent(value: String) {
        nativeSetJustifyContent(ref, value)
    }

    fun nativeSetAlignItems(value: String) {
        nativeSetAlignItems(ref, value)
    }

    private external fun nativeSetFlexDirection(fbLayout: Long, value: String)
    private external fun nativeSetJustifyContent(fbLayout: Long, value: String)
    private external fun nativeSetAlignItems(fbLayout: Long, value: String)

}