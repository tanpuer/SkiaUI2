package com.temple.skiaui.bitmap

import com.temple.skiaui.HYSkiaEngine

class AndroidBitmapLoader(val engine: HYSkiaEngine, val ref: Long) {

    private var imageLoader: ImageLoader = GlideImageLoader(engine, ref)

    fun setSource(source: String, viewWidth: Int, viewHeight: Int) {
        imageLoader.requestBitmap(source, viewWidth, viewHeight)
    }

    fun setResId(resId: Int, viewWidth: Int, viewHeight: Int) {
        imageLoader.requestDrawable(resId, viewWidth, viewHeight)
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