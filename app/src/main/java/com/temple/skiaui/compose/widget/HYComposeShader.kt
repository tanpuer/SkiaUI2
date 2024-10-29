package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource

class HYComposeShader(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Shader"

    override fun initAttrs(modifier: Modifier) {
        super.initAttrs(modifier)
        modifier.attributes.forEach { (key, value) ->
            when (key) {
                "shaderSource" -> {
                    val source = value as ShaderSource
                    nativeSetSource(ref, source.source, source.list)
                }

            }
        }
    }

    private external fun nativeSetSource(shaderView: Long, source: String, images: Array<String>)

}