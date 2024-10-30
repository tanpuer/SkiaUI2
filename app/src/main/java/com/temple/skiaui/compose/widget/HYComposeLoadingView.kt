package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeLoadingView(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Loading"

    fun setColor(color: String) {
        if (color.isEmpty()) {
            return
        }
        nativeSetColor(ref, color)
    }

    private external fun nativeSetColor(loadingView: Long, color: String)

}