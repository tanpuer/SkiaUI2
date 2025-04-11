package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource

class HYComposeShader(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String = "Shader"

    fun setShaderSource(shaderSource: ShaderSource) {
        nativeSetSource(ref, shaderSource.source, shaderSource.list)
    }

    private external fun nativeSetSource(shaderView: Long, source: String, images: Array<String>)

}