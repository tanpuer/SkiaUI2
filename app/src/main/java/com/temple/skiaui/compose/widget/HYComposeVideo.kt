package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeVideo(modifier: Modifier): HYComposeView(modifier)  {

    override fun createComposeView(): HYComposeView {
        ref = nativeCreateView(contextPtr)
        return this
    }

    override fun initAttrs(view: HYComposeView, modifier: Modifier) {
        super.initAttrs(view, modifier)
        modifier.attributes.forEach { (key, value) ->
            when (key) {
                "source" -> {
                    nativeSetSource(ref, value as String)
                }

            }
        }
    }

    private external fun nativeCreateView(context: Long): Long
    private external fun nativeSetSource(videoView: Long, source: String)

}