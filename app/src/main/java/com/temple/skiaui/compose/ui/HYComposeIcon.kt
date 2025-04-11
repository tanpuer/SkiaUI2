package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeIcon(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Icon"



    fun setIcon(icon: Int) {
        nativeSetIcon(ref, icon)
    }

    fun setIconSize(size: Int) {
        nativeSetSize(ref, size)
    }

    fun setIconColor(color: String) {
        nativeSetColor(ref, color)
    }

    private external fun nativeSetIcon(ref: Long, icon: Int)
    private external fun nativeSetSize(ref: Long, size: Int)
    private external fun nativeSetColor(ref: Long, color: String)
}