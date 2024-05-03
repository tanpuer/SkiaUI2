package com.temple.skiaui

import android.content.res.AssetManager
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.VelocityTracker
import com.temple.skiaui.plugin.PluginManager
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

    private val refreshRate = HYSkiaUIApp.getInstance().getFrameRate()
    private var frameCount = 0
    private var drawCount = AtomicInteger(0)
    var renderCallback: RenderCallback? = null
    private val start = System.currentTimeMillis()
    private val pluginManager = PluginManager()
    private var glApp = 0L
    private var uiApp = 0L

    init {
        skiaGLHandler.post {
            glApp = nativeGLInit()
        }
        skiaUIHandler.post {
            uiApp = nativeUIInit(HYSkiaUIApp.getInstance().assets)
            nativeSetPlugins(pluginManager)
        }
    }

    fun createSurface(surface: Surface) {
        skiaGLHandler.post {
            nativeGLCreated(glApp, surface)
        }
        velocityTracker = VelocityTracker.obtain()
    }

    fun changeSurfaceSize(width: Int, height: Int) {
        skiaGLHandler.post {
            nativeGLChanged(glApp, width, height, System.currentTimeMillis() / 1000)
        }
        skiaUIHandler.post {
            nativeUIChanged(uiApp, width, height, System.currentTimeMillis() / 1000)
            pic.set(nativeUIDoFrame(uiApp, System.currentTimeMillis() - start))
        }
    }

    fun destroySurface() {
        skiaGLHandler.post {
            nativeGLDestroyed(glApp)
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
            pic.set(nativeUIDoFrame(uiApp, System.currentTimeMillis() - start))
            finishDraw.set(true)
            drawCount.set(drawCount.get() + 1)
        }
        skiaGLHandler.post {
            if (pic.get() != 0L) {
                nativeGLDoFrame(glApp, pic.get(), time)
                pic.set(0L)
            }
        }
    }

    fun dispatchHYTouchEvent(event: MotionEvent): Boolean {
        val x = event.x
        val y = event.y
        val action = event.action
        skiaUIHandler.post {
            nativeTouchEvent(uiApp, action, x, y)
        }
        velocityTracker?.addMovement(event)
        if (action == MotionEvent.ACTION_UP) {
            velocityTracker?.computeCurrentVelocity(1000)
            skiaUIHandler.post {
                nativeSetVelocity(
                    uiApp,
                    velocityTracker?.xVelocity ?: 0f,
                    velocityTracker?.yVelocity ?: 0f
                )
            }
        }
        return true
    }

    fun release() {
        nativeRelease(uiApp, glApp)
        skiaUIHandlerThread.quitSafely()
        skiaGLHandlerThread.quitSafely()
        uiApp = 0L
        glApp = 0L
    }

    fun onBackPressed() {
        skiaUIHandler.post {
            if (!nativeBackPressed(uiApp)) {
                Handler(Looper.getMainLooper()).post {
                    renderCallback?.onPlatformBackPressed()
                }
            }
        }
    }

    private external fun nativeGLInit(): Long
    private external fun nativeGLCreated(glApp: Long, surface: Surface)
    private external fun nativeGLChanged(glApp: Long, width: Int, height: Int, time: Long)
    private external fun nativeGLDestroyed(glApp: Long)
    private external fun nativeGLDoFrame(glApp: Long, pic: Long, time: Long)
    private external fun nativeUIInit(assets: AssetManager): Long
    private external fun nativeTouchEvent(uiApp: Long, action: Int, x: Float, y: Float): Boolean
    private external fun nativeSetVelocity(uiApp: Long, xVelocity: Float, yVelocity: Float)
    private external fun nativeUIChanged(uiApp: Long, width: Int, height: Int, time: Long)
    private external fun nativeUIDoFrame(uiApp: Long, time: Long): Long
    private external fun nativeBackPressed(uiApp: Long): Boolean
    private external fun nativeRelease(uiApp: Long, glApp: Long)
    private external fun nativeSetPlugins(pluginManager: PluginManager)

    companion object {
        init {
            System.loadLibrary("skiaui")
        }

        private const val TAG = "SkiaUI"
    }

}