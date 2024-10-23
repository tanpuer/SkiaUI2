package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeVideo(modifier: Modifier): HYComposeView(modifier)  {

    override fun createComposeView(): HYComposeView {
        //jni create video.cpp
        return super.createComposeView()
    }

    fun setSource(source: String) {
        //jni set source
    }
}