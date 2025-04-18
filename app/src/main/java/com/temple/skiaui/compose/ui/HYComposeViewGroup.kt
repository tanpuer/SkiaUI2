package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.StyleKey

open class HYComposeViewGroup(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "ViewGroup"

    override fun innerUpdateModifier(modifier: Modifier) {
        super.innerUpdateModifier(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                StyleKey.flexDirection -> {
                    nativeSetFlexDirection(ref, (value as FlexDirection).value)
                }

                StyleKey.justifyContent -> {
                    nativeSetJustifyContent(ref, (value as Justify).value)
                }

                StyleKey.alignItems -> {
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