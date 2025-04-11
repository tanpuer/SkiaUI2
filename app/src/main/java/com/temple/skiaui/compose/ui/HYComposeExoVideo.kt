package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeExoVideo(modifier: Modifier) : HYComposeView(modifier) {

    fun setSource(source: String) {
        nativeSetSource(ref, source)
    }

    override fun getViewType(): String = "ExoVideo"

    private external fun nativeSetSource(videoView: Long, source: String)

}