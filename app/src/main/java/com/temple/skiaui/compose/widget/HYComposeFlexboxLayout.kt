package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeFlexboxLayout(modifier: Modifier): HYComposeView(modifier) {

    override fun createComposeView(): HYComposeView {
        //jni create flexboxlayout.cpp
        return this
    }
}