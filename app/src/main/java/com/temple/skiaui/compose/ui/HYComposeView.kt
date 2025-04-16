package com.temple.skiaui.compose.ui

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.colorspace.ColorSpace
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.ui.util.composeColorToSkiaColor

open class HYComposeView(modifier: Modifier) : HYComposeNode(modifier) {

    override fun initStyles(modifier: Modifier) {
        super.initStyles(modifier)
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "size" -> {
                    val size = value as IntArray
                    setSize(size[0], size[1])
                }

                "width" -> {
                    nativeSetWidth(ref, value as Int)
                }

                "height" -> {
                    nativeSetHeight(ref, value as Int)
                }

                "backgroundColor" -> {
                    setBackgroundColor(value as Color)
                }

                "margins" -> {
                    nativeSetMargins(ref, value as IntArray)
                }

                "paddings" -> {
                    nativeSetPaddings(ref, value as IntArray)
                }

                "position" -> {
                    nativeSetPosition(ref, (value as Position).value)
                }

                "corner" -> {
                    nativeSetCornerRadius(ref, value as Int)
                }

                "minSize" -> {
                    nativeSetMinSize(ref, (value as IntArray)[0], value[1])
                }
            }
        }
    }

    override fun initEvents(modifier: Modifier) {
        super.initEvents(modifier)
        modifier.events.forEach { (key, value) ->
            when (key) {
                "click" -> {
                    nativeSetClickCallback(ref)
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
        (modifier.events["click"])?.invoke(this, null)
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
}