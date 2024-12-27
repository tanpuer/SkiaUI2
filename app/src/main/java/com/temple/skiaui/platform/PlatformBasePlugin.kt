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
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R

abstract class PlatformBasePlugin(
    val engine: HYSkiaEngine,
    val width: Int,
    val height: Int,
    val viewPtr: Long
) : ICanvasProvider {

    protected val mainHandler = Handler(Looper.getMainLooper())

    protected var targetView: View? = null

    private var container: FrameLayout? = null

    private var surface: Surface? = null

    private var imageReader: ImageReader? = null

    private var skImagePtr: Long = 0L

    private var downTime: Long = 0L

    init {
        imageReader = ImageReader.newInstance(
            width, height, ImageFormat.PRIVATE, 1, HardwareBuffer.USAGE_GPU_SAMPLED_IMAGE
        )
        surface = imageReader?.surface
        mainHandler.post {
            container = (engine.view.parent as ViewGroup).findViewById(R.id.platformContainer)
            targetView = initPlatformView()
            container?.addView(targetView, ViewGroup.LayoutParams(width, height))
        }
    }

    abstract fun initPlatformView(): View

    abstract fun destroyPlatformView()

    fun getSkImage(): Long {
        return skImagePtr
    }

    fun release() {
        mainHandler.post {
            container?.removeView(targetView)
            destroyPlatformView()
            targetView = null
        }
    }

    fun sendTouchEvent(type: Int, x: Float, y: Float) {
        mainHandler.post {
            if (type == MotionEvent.ACTION_DOWN) {
                downTime = System.currentTimeMillis()
            }
            val eventTime = System.currentTimeMillis()
            val motionEvent = MotionEvent.obtain(
                downTime,
                eventTime,
                type,
                x,
                y,
                0
            )
            Log.d("PlatformBasePlugin", "touch-event: $type $x $y")
            targetView?.onTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    override fun lockCanvas(): Canvas? {
        return surface?.lockHardwareCanvas()
    }

    override fun unLockCanvas(canvas: Canvas) {
        surface?.unlockCanvasAndPost(canvas)
        val hardwareBuffer = getLatestHardwareBuffer() ?: return
        engine.makeHardwareBufferToSkImage(hardwareBuffer) {
            if (skImagePtr != 0L) {
                deleteSkImage(skImagePtr)
            }
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

    private fun deleteSkImage(ptr: Long) {
        engine.deleteSkImage(ptr)
    }
}