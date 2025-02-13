package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeWeb(modifier: Modifier) : HYComposeView(modifier) {

    fun loadUrl(url: String) {
        nativeLoadUrl(ref, url)
    }

    override fun getViewType(): String = "Web"

    private external fun nativeLoadUrl(videoView: Long, url: String)

}