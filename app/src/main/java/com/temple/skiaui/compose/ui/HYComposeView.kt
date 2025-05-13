package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.StyleKey
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor
import com.temple.skiaui.compose.ui.util.dp2px
import com.temple.skiaui.compose.ui.util.dpArrayToIntArray

open class HYComposeView(modifier: Modifier) : HYComposeNode(modifier) {

    private var clickCallback: (() -> Unit)? = null

    override fun innerUpdateModifier(modifier: Modifier) {
        modifier.styles.forEach { (key, value) ->
            when (key) {
                StyleKey.width -> {
                    nativeSetWidth(ref, dp2px(value as Dp))
                }

                StyleKey.height -> {
                    nativeSetHeight(ref, dp2px(value as Dp))
                }

                StyleKey.backgroundColor -> {
                    setBackgroundColor(value as Color)
                }

                StyleKey.margins -> {
                    nativeSetMargins(ref, dpArrayToIntArray(value as Array<Dp>))
                }

                StyleKey.paddings -> {
                    nativeSetPaddings(ref, dpArrayToIntArray(value as Array<Dp>))
                }

                StyleKey.position -> {
                    nativeSetPosition(ref, (value as Position).value)
                }

                StyleKey.corner -> {
                    nativeSetCornerRadius(ref, dp2px(value as Dp))
                }

                StyleKey.minSize -> {
                    val size = value as Array<*>
                    nativeSetMinSize(ref, dp2px(size[0] as Dp), dp2px(size[1] as Dp))
                }

                StyleKey.alignSelf -> {
                    nativeSetAlignSelf(ref, (value as Align).value)
                }

                StyleKey.widthPercent -> {
                    nativeSetWidthPercent(ref, value as Float)
                }

                StyleKey.heightPercent -> {
                    nativeSetHeightPercent(ref, value as Float)
                }

                StyleKey.flex -> {
                    nativeSetFlex(ref, value as Int)
                }
            }
        }
    }

    fun setSize(width: Int, height: Int) {
        nativeSetWidth(ref, width)
        nativeSetHeight(ref, height)
    }

    fun setBackgroundColor(color: Color) {
        nativeSetBackgroundColor(ref, composeColorToSkiaColor(color))
    }

    fun setWidth(width: Int) {
        nativeSetWidth(ref, width)
    }

    fun setHeight(height: Int) {
        nativeSetHeight(ref, height)
    }

    fun setRotateZ(rotateZ: Float) {
        nativeSetRotateZ(ref, rotateZ)
    }

    fun setMargins(margins: IntArray) {
        nativeSetMargins(ref, margins)
    }

    fun triggerClickEvent() {
        clickCallback?.invoke()
    }

    fun setOnClick(callback: (() -> Unit)? = null) {
        clickCallback = callback
        callback?.let {
            nativeSetClickCallback(ref)
        }
    }

    override fun getViewType(): String = "View"

    private external fun nativeSetWidth(view: Long, width: Int)
    private external fun nativeSetHeight(view: Long, height: Int)
    private external fun nativeSetBackgroundColor(view: Long, color: Int)
    private external fun nativeSetClickCallback(view: Long)
    private external fun nativeSetRotateZ(view: Long, rotateZ: Float)
    private external fun nativeSetMargins(view: Long, margins: IntArray)
    private external fun nativeSetPaddings(view: Long, padding: IntArray)
    private external fun nativeSetPosition(view: Long, position: String)
    private external fun nativeSetCornerRadius(view: Long, radius: Int)
    private external fun nativeSetMinSize(view: Long, minWidth: Int, minHeight: Int)
    private external fun nativeSetAlignSelf(view: Long, value: String)
    private external fun nativeSetWidthPercent(view: Long, percent: Float)
    private external fun nativeSetHeightPercent(view: Long, percent: Float)
    private external fun nativeSetFlex(view: Long, flex: Int)
}