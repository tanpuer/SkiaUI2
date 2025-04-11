package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeLottie(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Lottie"

    fun setSource(source: String) {
        nativeSetSource(ref, source)
    }

    fun start() {
        nativeStart(ref)
    }

    fun pause() {
        nativePause(ref)
    }

    private external fun nativeSetSource(lottieView: Long, source: String)
    private external fun nativeStart(lottieView: Long)
    private external fun nativePause(lottieView: Long)

}