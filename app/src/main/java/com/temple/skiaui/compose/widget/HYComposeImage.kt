package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeImage(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Image"

    fun setImageSource(source: String) {
        nativeSetSource(ref, source)
    }

    private external fun nativeSetSource(shaderView: Long, source: String)

}