package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeSwitch(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Switch"

    fun setEnable(enable: Boolean) {
        nativeSetEnable(ref, enable)
    }

    fun setColor(color: String) {
        nativeSetColor(ref, color)
    }

    private external fun nativeSetEnable(ref: Long, enable: Boolean)
    private external fun nativeSetColor(ref: Long, color: String)
}