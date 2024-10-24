package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeVideo(modifier: Modifier): HYComposeView(modifier)  {

    override fun createComposeView(): HYComposeView {
        ref = nativeCreateView()
        return this
    }

    fun setSource(source: String) {
        nativeSetSource(ref, source)
    }

    private external fun nativeCreateView(): Long
    private external fun nativeSetSource(videoView: Long, source: String)

}