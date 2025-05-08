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

                StyleKey.wrap -> {
                    nativeSetFlexWrap(ref, (value as FlexWrap).value)
                }

            }
        }
    }

    fun setFlexDirection(direction: FlexDirection) {
        nativeSetFlexDirection(ref, direction.value)
    }

    private external fun nativeSetFlexDirection(ref: Long, value: String)
    private external fun nativeSetJustifyContent(ref: Long, value: String)
    private external fun nativeSetAlignItems(ref: Long, value: String)
    private external fun nativeSetFlexWrap(ref: Long, value: String)

}