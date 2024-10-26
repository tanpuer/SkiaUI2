package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeLoadingView(modifier: Modifier): HYComposeView(modifier) {

    override fun createComposeView(): HYComposeView {
        ref = nativeCreateView(contextPtr)
        return this
    }

    private external fun nativeCreateView(context: Long): Long
}