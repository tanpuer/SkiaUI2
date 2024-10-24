package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeView(modifier: Modifier) : HYComposeNode(modifier) {

    override fun createComposeView(): HYComposeView {
        ref = nativeCreateView()
        return this
    }

    fun setSize(width: Int, height: Int) {
        nativeSetWidth(ref, width)
        nativeSetHeight(ref, height)
    }

    fun setBackgroundColor(color: String) {
        nativeSetBackgroundColor(ref, color)
    }

    private external fun nativeCreateView(): Long
    private external fun nativeSetWidth(view: Long, width: Int)
    private external fun nativeSetHeight(view: Long, height: Int)
    private external fun nativeSetBackgroundColor(view: Long, color: String)
}