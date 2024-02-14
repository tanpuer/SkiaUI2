package com.temple.skiaui

import android.content.res.AssetManager
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.VelocityTracker
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

class HYSkiaEngine {

    private var velocityTracker: VelocityTracker? = null

    /**
     * 执行UI逻辑
     */
    private val skiaUIHandlerThread: HandlerThread =
        HandlerThread("skia-ui", Thread.MAX_PRIORITY).apply {
            start()
        }
    private val skiaUIHandler = Handler(skiaUIHandlerThread.looper)

    /**
     * 执行渲染逻辑
     */
    private val skiaGLHandlerThread: HandlerThread =
        HandlerThread("skia-gl", Thread.MAX_PRIORITY).apply {
            start()
        }
    private val skiaGLHandler = Handler(skiaGLHandlerThread.looper)

    private var finishDraw = AtomicBoolean(true)

    private var pic = AtomicLong(0)

    private val refreshRate = 60
    private var frameCount = 0
    private var drawCount = AtomicInteger(0)
    var renderCallback: RenderCallback? = null

    init {
        skiaGLHandler.post {
            nativeInit(HYSkiaUIApp.getInstance().assets)
        }
        skiaUIHandler.post {
            nativeUIInit()
        }
    }

    fun createSurface(surface: Surface) {
        skiaGLHandler.post {
            nativeSurfaceCreated(surface)
        }
        velocityTracker = VelocityTracker.obtain()
    }

    fun changeSurfaceSize(width: Int, height: Int) {
        skiaGLHandler.post {
            nativeSurfaceChanged(width, height, System.currentTimeMillis() / 1000)
        }
        skiaUIHandler.post {
            nativeUIChanged(width, height, System.currentTimeMillis() / 1000)
        }
    }

    fun destroySurface() {
        skiaGLHandler.post {
            nativeSurfaceDestroyed()
        }
        velocityTracker?.recycle()
        velocityTracker = null
    }

    fun doFrame(time: Long) {
        frameCount++
        if (frameCount == refreshRate) {
            renderCallback?.updateFps(drawCount.get())
            drawCount.set(0)
            frameCount = 0
        }
        if (!finishDraw.get()) {
            Log.d(TAG, "doFrame ignore current vysnc draw")
            return
        }
        skiaUIHandler.post {
            finishDraw.set(false)
            pic.set(nativeUIDoFrame(time))
            finishDraw.set(true)
            drawCount.set(drawCount.get() + 1)
        }
        skiaGLHandler.post {
            if (pic.get() != 0L) {
                nativeSurfaceDoFrame(pic.get(), time)
                pic.set(0L)
            }
        }
    }

    fun dispatchHYTouchEvent(event: MotionEvent): Boolean {
        skiaUIHandler.post {
            nativeTouchEvent(event.action, event.x, event.y)
        }
        velocityTracker?.addMovement(event)
        if (event.action == MotionEvent.ACTION_UP) {
            velocityTracker?.computeCurrentVelocity(1000)
            skiaUIHandler.post {
                nativeSetVelocity(
                    velocityTracker?.xVelocity ?: 0f,
                    velocityTracker?.yVelocity ?: 0f
                )
            }
        }
        return true
    }

    fun release() {
        nativeRelease()
        skiaUIHandlerThread.quitSafely()
        skiaGLHandlerThread.quitSafely()
    }

    private external fun nativeInit(assets: AssetManager)
    private external fun nativeSurfaceCreated(surface: Surface)
    private external fun nativeSurfaceChanged(width: Int, height: Int, time: Long)
    private external fun nativeSurfaceDestroyed()
    private external fun nativeSurfaceDoFrame(pic: Long, time: Long)

    private external fun nativeTouchEvent(action: Int, x: Float, y: Float): Boolean
    private external fun nativeSetVelocity(xVelocity: Float, yVelocity: Float)
    private external fun nativeUIInit()
    private external fun nativeUIChanged(width: Int, height: Int, time: Long)
    private external fun nativeUIDoFrame(time: Long): Long

    private external fun nativeRelease()

    companion object {
        init {
            System.loadLibrary("skiaui")
        }

        private const val TAG = "SkiaUI"
    }

}