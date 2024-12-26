package com.temple.skiaui.platform

import android.graphics.Canvas
import android.graphics.ImageFormat
import android.hardware.HardwareBuffer
import android.media.ImageReader
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.ViewGroup
import android.widget.FrameLayout
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R

class PlatformWebViewPlugin(val engine: HYSkiaEngine, width: Int, height: Int, webViewPtr: Long) :
    IWebViewCallback {

    private val mainHandler = Handler(Looper.getMainLooper())

    private var webView: PlatformWebView? = null

    private var container: FrameLayout? = null

    private var surface: Surface? = null

    private var imageReader: ImageReader? = null

    private var skImagePtr: Long = 0L

    private var webViewPtr: Long = 0L

    init {
        this.webViewPtr = webViewPtr
        imageReader = ImageReader.newInstance(
            width, height, ImageFormat.PRIVATE, 2, HardwareBuffer.USAGE_GPU_SAMPLED_IMAGE
        )
        surface = imageReader?.surface
        mainHandler.post {
            container = (engine.view.parent as ViewGroup).findViewById(R.id.platformContainer)
            webView = PlatformWebView(engine.view.context)
            webView?.setPlatformRenderer(this)
            container?.addView(webView, ViewGroup.LayoutParams(width, height))
            webView?.callback = this
        }
    }

    fun getSkImage(): Long {
        return skImagePtr
    }

    fun loadUrl(url: String) {
        mainHandler.post {
            webView?.loadUrl(url)
        }
    }

    fun sendTouchEvent(type: Int, x: Float, y: Float) {
        mainHandler.post {
            val eventTime = System.currentTimeMillis()
            val motionEvent = MotionEvent.obtain(
                eventTime,
                eventTime,
                type,
                x,
                y,
                0
            )
            Log.d(TAG, "touch-event: $type $x $y")
            webView?.onTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    fun release() {
        mainHandler.post {
            container?.removeView(webView)
            webView?.destroy()
            webView = null
        }
        engine.postToSkiaUI {
            webViewPtr = 0L
        }
    }

    fun lockCanvas(): Canvas? {
        return surface?.lockHardwareCanvas()
    }

    fun unLockCanvas(canvas: Canvas) {
        surface?.unlockCanvasAndPost(canvas)
        val hardwareBuffer = getLatestHardwareBuffer() ?: return
        engine.makeHardwareBufferToSkImage(hardwareBuffer) {
            skImagePtr = it
        }
    }

    private fun getLatestHardwareBuffer(): HardwareBuffer? {
        val image = imageReader?.acquireLatestImage()
        if (image != null) {
            val hardwareBuffer = image.hardwareBuffer
            image.close()
            return hardwareBuffer
        }
        return null
    }

    companion object {
        const val TAG = "HYWebView"
    }

    override fun onProgressChanged(progress: Int) {
        engine.postToSkiaUI {
            if (webViewPtr != 0L) {
                engine.webViewProgressChange(webViewPtr, progress)
            }
        }
    }

}
