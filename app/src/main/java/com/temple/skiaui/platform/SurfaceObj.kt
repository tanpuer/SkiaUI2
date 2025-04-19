package com.temple.skiaui.platform

import android.graphics.SurfaceTexture
import android.view.Surface

class SurfaceObj {

    var surfaceTexture: SurfaceTexture? = null

    var surface: Surface? = null

    var width = 0

    var height = 0

    fun release() {
        surfaceTexture?.setOnFrameAvailableListener(null)
        surfaceTexture?.release()
        surface?.release()
    }

    fun setDefaultBufferSize(width: Int, height: Int) {
        this.width = width
        this.height = height
        surfaceTexture?.setDefaultBufferSize(width, height)
    }

    companion object {
        var INDEX = 0
    }

}
