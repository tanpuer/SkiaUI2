package com.temple.skiaui.platform

import android.graphics.ImageFormat
import android.hardware.HardwareBuffer
import android.media.Image.Plane
import android.media.ImageReader
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.util.Log
import android.view.Surface
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.data.ImageReaderYUVData

abstract class PlatformImageReaderBasePlugin(
    val engine: HYSkiaEngine,
    val width: Int,
    val height: Int
) {

    protected var imageReader: ImageReader? = null

    protected var surface: Surface? = null

    protected val mainHandler = Handler(Looper.getMainLooper())

    protected val imageReaderThread = HandlerThread("${TAG}:${INDEX++}").apply { start() }

    protected val imageReaderHandler = Handler(imageReaderThread.looper)

    protected var yuvData: ImageReaderYUVData? = null

    init {
        imageReaderHandler.post {
            initImageReader()
        }
    }

    abstract fun copyYUVData(planes: Array<Plane>, width: Int, height: Int): ImageReaderYUVData

    abstract fun onSurfaceCreated(surface: Surface?)

    abstract fun onSurfaceDestroyed(surface: Surface?)

    fun getYUVData(): ImageReaderYUVData? {
        return yuvData
    }

    fun onShow() {
        imageReaderHandler.post {
            innerOnShow()
        }
    }

    fun onHide() {
        imageReaderHandler.post {
            innerOnHide()
        }
    }

    open fun innerOnShow() {}

    open fun innerOnHide() {}

    open fun release() {
        imageReaderHandler.post {
            onSurfaceDestroyed(surface)
            imageReader?.close()
            surface = null
            imageReader = null
        }
        imageReaderThread.quitSafely()
    }

    open fun getImageFormat(): Int {
        return ImageFormat.YUV_420_888
    }

    private fun initImageReader() {
        imageReader = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            ImageReader.newInstance(
                width, height, getImageFormat(), 2, HardwareBuffer.USAGE_GPU_SAMPLED_IMAGE
            )
        } else {
            ImageReader.newInstance(
                width, height, getImageFormat(), 2
            )
        }
        imageReader?.setOnImageAvailableListener({ reader ->
            val image = reader?.acquireLatestImage()
            Log.d(TAG, "${(image?.timestamp ?: 0) / 1000000}")
            image?.planes?.let {
                val data = copyYUVData(it, image.width, image.height)
                engine.postToSkiaUI {
                    yuvData = data
                }
            }
            image?.close()
        }, imageReaderHandler)
        surface = imageReader?.surface
        onSurfaceCreated(surface)
    }

    companion object {
        private var INDEX = 1
        private const val TAG = "HYImageReader"
    }
}
