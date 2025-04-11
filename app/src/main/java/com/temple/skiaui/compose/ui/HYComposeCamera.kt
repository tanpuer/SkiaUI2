package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

interface CameraCallback {
    fun onImageCaptured(imagePtr: Long)
}

class HYComposeCamera(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String {
        return "Camera"
    }

    fun capture(callback: CameraCallback) {
        nativeCapture(ref, callback)
    }

    private external fun nativeCapture(ref: Long, callback: CameraCallback)
}