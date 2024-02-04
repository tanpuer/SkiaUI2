package com.temple.skiaui

import android.content.res.AssetManager
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import android.view.Choreographer
import android.view.MotionEvent
import android.view.Surface
import android.view.VelocityTracker
import java.util.concurrent.atomic.AtomicBoolean

class HYSkiaEngine : Choreographer.FrameCallback {

    private var velocityTracker: VelocityTracker? = null

    private val skiaUIHandlerThread: HandlerThread =
        HandlerThread("skia-ui", Thread.MAX_PRIORITY).apply {
            start()
        }
    private val skiaUIHandler = Handler(skiaUIHandlerThread.looper)

    private var finishDraw = AtomicBoolean(true)

    init {
        skiaUIHandler.post {
            nativeInit(HYSkiaUIApp.getInstance().assets)
        }
    }

    fun createSurface(surface: Surface) {
        skiaUIHandler.post {
            nativeSurfaceCreated(surface)
        }
        Choreographer.getInstance().postFrameCallback(this)
        velocityTracker = VelocityTracker.obtain()
    }

    fun changeSurfaceSize(width: Int, height: Int) {
        skiaUIHandler.post {
            nativeSurfaceChanged(width, height, System.currentTimeMillis() / 1000)
        }
    }

    fun destroySurface() {
        Choreographer.getInstance().removeFrameCallback(this)
        skiaUIHandler.post {
            nativeSurfaceDestroyed()
        }
        velocityTracker?.recycle()
        velocityTracker = null
    }

    override fun doFrame(time: Long) {
        if (!finishDraw.get()) {
            Log.d(TAG, "doFrame ignore current vysnc draw")
            return
        }
        skiaUIHandler.post {
            nativeSurfaceDoFrame(time / 1000000)
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    fun dispatchHYTouchEvent(event: MotionEvent): Boolean {
        nativeTouchEvent(event.action, event.x, event.y)
        velocityTracker?.addMovement(event)
        if (event.action == MotionEvent.ACTION_UP) {
            velocityTracker?.computeCurrentVelocity(1000)
            nativeSetVelocity(
                velocityTracker?.xVelocity ?: 0f,
                velocityTracker?.yVelocity ?: 0f
            )
        }
        return true
    }

    private external fun nativeInit(assets: AssetManager)
    private external fun nativeSurfaceCreated(surface: Surface)
    private external fun nativeSurfaceChanged(width: Int, height: Int, time: Long)
    private external fun nativeSurfaceDestroyed()
    private external fun nativeSurfaceDoFrame(time: Long)
    private external fun nativeTouchEvent(action: Int, x: Float, y: Float): Boolean
    private external fun nativeSetVelocity(xVelocity: Float, yVelocity: Float)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }

        private const val TAG = "SkiaUI"
    }

}