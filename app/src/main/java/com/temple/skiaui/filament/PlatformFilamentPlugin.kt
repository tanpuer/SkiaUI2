package com.temple.skiaui.filament

import android.media.Image
import android.view.Surface
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformImageReaderBasePlugin
import com.temple.skiaui.platform.data.ImageReaderYUVData

class PlatformFilamentPlugin(engine: HYSkiaEngine, width: Int, height: Int) :
    PlatformImageReaderBasePlugin(engine, width, height) {

    override fun copyYUVData(
        planes: Array<Image.Plane>,
        width: Int,
        height: Int
    ): ImageReaderYUVData {
        TODO("Not yet implemented")
    }

    override fun onSurfaceCreated(surface: Surface?) {
    }

    override fun onSurfaceDestroyed(surface: Surface?) {
    }

}