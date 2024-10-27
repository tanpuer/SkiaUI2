package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeLottie(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Lottie"

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

    private external fun nativeSetSource(lottieView: Long, source: String)

}