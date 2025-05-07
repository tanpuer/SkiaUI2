package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

abstract class HYComposeBaseSurfaceTexture(modifier: Modifier) : HYComposeView(modifier) {

    fun setShaderPath(path: String?) {
        if (path.isNullOrEmpty()) {
            return
        }
        nativeSetShaderPath(ref, path)
    }

    fun setShaderCode(code: String?) {
        if (code.isNullOrEmpty()) {
            return
        }
        nativeSetShaderCode(ref, code)
    }

    private external fun nativeSetShaderPath(ref: Long, path: String)
    private external fun nativeSetShaderCode(ref: Long, code: String)
}