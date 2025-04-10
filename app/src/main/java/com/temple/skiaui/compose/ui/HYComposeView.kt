package com.temple.skiaui.compose.ui

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

                "width" -> {
                    nativeSetWidth(ref, value as Int)
                }

                "height" -> {
                    nativeSetHeight(ref, value as Int)
                }

                "backgroundColor" -> {
                    nativeSetBackgroundColor(ref, value as String)
                }

                "margins" -> {
                    nativeSetMargins(ref, value as IntArray)
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

    fun setBackgroundColor(color: String) {
        nativeSetBackgroundColor(ref, color)
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
    private external fun nativeSetBackgroundColor(view: Long, color: String)
    private external fun nativeSetClickCallback(view: Long)
    private external fun nativeSetRotateZ(view: Long, rotateZ: Float)
    private external fun nativeSetMargins(view: Long, margins: IntArray)
}