package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeSVG(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "SVG"

    fun setSource(source: String) {
        nativeSetSource(ref, source)
    }

    private external fun nativeSetSource(svg: Long, source: String)

}