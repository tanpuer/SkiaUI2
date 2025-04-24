package com.temple.skiaui.bitmap

import com.temple.skiaui.HYSkiaEngine

class AndroidBitmapLoader(val engine: HYSkiaEngine, val ref: Long) {

    private var imageLoader: ImageLoader = GlideImageLoader(engine, ref)

    fun setSource(source: String) {
        imageLoader.requestBitmap(source)
    }

    fun setResId(resId: Int) {
        imageLoader.requestDrawable(resId)
    }

    fun release() {
        imageLoader.release()
    }

    private fun start() {
        imageLoader.start()
    }

    private fun stop() {
        imageLoader.stop()
    }

    companion object {
        private const val TAG = "AndroidBitmap"
    }

}