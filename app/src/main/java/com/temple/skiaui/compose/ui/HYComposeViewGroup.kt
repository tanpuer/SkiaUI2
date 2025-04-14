package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeViewGroup(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "ViewGroup"

    override fun initStyles(modifier: Modifier) {
        super.initStyles(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "flexDirection" -> {
                    nativeSetFlexDirection(ref, (value as FlexDirection).value)
                }

                "justifyContent" -> {
                    nativeSetJustifyContent(ref, (value as Justify).value)
                }

                "alignItems" -> {
                    nativeSetAlignItems(ref, (value as Align).value)
                }

            }
        }
    }

    fun setFlexDirection(direction: FlexDirection) {
        nativeSetFlexDirection(ref, direction.value)
    }

    fun setJustifyContent(justify: Justify) {
        nativeSetJustifyContent(ref, justify.value)
    }

    fun nativeSetAlignItems(align: Align) {
        nativeSetAlignItems(ref, align.value)
    }

    private external fun nativeSetFlexDirection(fbLayout: Long, value: String)
    private external fun nativeSetJustifyContent(fbLayout: Long, value: String)
    private external fun nativeSetAlignItems(fbLayout: Long, value: String)

}