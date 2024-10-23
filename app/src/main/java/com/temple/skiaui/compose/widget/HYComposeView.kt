package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeView(modifier: Modifier) : HYComposeNode(modifier) {

    override fun createComposeView(): HYComposeView {
        //jni create view
        return this
    }

    fun setSize(width: Int, height: Int) {
        //jni
    }

    fun setBackgroundColor(color: String) {
        //jni
    }

}