package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeVideo(modifier: Modifier) : HYComposeView(modifier) {

    override fun initAttrs(modifier: Modifier) {
        super.initAttrs(modifier)
        modifier.attributes.forEach { (key, value) ->
            when (key) {
                "source" -> {
                    nativeSetSource(ref, value as String)
                }

            }
        }
    }

    override fun getViewType(): String = "Video"

    private external fun nativeSetSource(videoView: Long, source: String)

}