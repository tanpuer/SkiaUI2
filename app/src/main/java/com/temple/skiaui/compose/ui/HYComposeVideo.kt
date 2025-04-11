package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeVideo(modifier: Modifier) : HYComposeView(modifier) {

    fun setSource(source: String) {
        nativeSetSource(ref, source)
    }

    override fun getViewType(): String = "Video"

    private external fun nativeSetSource(videoView: Long, source: String)

}