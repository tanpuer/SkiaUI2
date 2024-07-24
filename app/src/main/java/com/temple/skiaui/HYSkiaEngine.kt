package com.temple.skiaui

import android.content.res.AssetManager
import android.hardware.HardwareBuffer
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.VelocityTracker
import com.temple.skiaui.plugin.PluginManager
import java.util.concurrent.Executors
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
    private val skImageList = mutableListOf<Long>()
    val createListeners = mutableListOf<(enable: Boolean) -> Unit>()
    private val executors = Executors.newFixedThreadPool(2)

    init {
        skiaGLHandler.post {
            glApp = nativeGLInit()
        }
        skiaUIHandler.post {
            uiApp = nativeUIInit(HYSkiaUIApp.getInstance().assets)
            nativeSetPlugins(uiApp, pluginManager)
        }
    }

    fun createSurface(surface: Surface) {
        pic.set(0L)
        finishDraw.set(true)
        drawCount.set(0)
        skiaUIHandler.post {
            nativeUIShow(uiApp)
        }
        skiaGLHandler.post {
            nativeGLCreated(glApp, surface)
        }
        velocityTracker = VelocityTracker.obtain()
        createListeners.forEach {
            it.invoke(true)
        }
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
        skiaUIHandler.post {
            nativeUIHide(uiApp)
        }
        skiaGLHandler.post {
            nativeGLDestroyed(glApp)
        }
        velocityTracker?.recycle()
        velocityTracker = null
        createListeners.forEach {
            it.invoke(false)
        }
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
                skImageList.forEach {
                    nativeDeleteSkImage(glApp, it)
                }
                skImageList.clear()
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

    fun makeHardwareBufferToSkImage(
        hardwareBuffer: HardwareBuffer,
        callback: (skImagePtr: Long) -> Unit
    ) {
        skiaGLHandler.post {
            val skImagePtr = nativeGLMakeHardwareBufferToSkImage(glApp, hardwareBuffer)
            skiaUIHandler.post {
                callback.invoke(skImagePtr);
            }
        }
    }

    fun deleteSkImage(skImagePtr: Long) {
        skiaGLHandler.post {
            skImageList.add(skImagePtr)
        }
    }

    fun executeTask(taskId: Int) {
        executors.submit {
            nativeExecuteTask(uiApp, taskId, HYSkiaUIApp.getInstance().assets)
        }
    }

    fun postTask(taskId: Int) {
        skiaUIHandler.post {
            nativePostTask(uiApp, taskId)
        }
    }

    private external fun nativeGLInit(): Long
    private external fun nativeGLCreated(glApp: Long, surface: Surface)
    private external fun nativeGLChanged(glApp: Long, width: Int, height: Int, time: Long)
    private external fun nativeGLDestroyed(glApp: Long)
    private external fun nativeGLDoFrame(glApp: Long, pic: Long, time: Long)
    private external fun nativeGLMakeHardwareBufferToSkImage(
        glApp: Long,
        hardwareBuffer: HardwareBuffer
    ): Long

    private external fun nativeDeleteSkImage(glApp: Long, skImagePtr: Long)

    private external fun nativeUIInit(assets: AssetManager): Long
    private external fun nativeTouchEvent(uiApp: Long, action: Int, x: Float, y: Float): Boolean
    private external fun nativeSetVelocity(uiApp: Long, xVelocity: Float, yVelocity: Float)
    private external fun nativeUIChanged(uiApp: Long, width: Int, height: Int, time: Long)
    private external fun nativeUIDoFrame(uiApp: Long, time: Long): Long
    private external fun nativeBackPressed(uiApp: Long): Boolean
    private external fun nativeRelease(uiApp: Long, glApp: Long)
    private external fun nativeSetPlugins(uiApp: Long, pluginManager: PluginManager)
    private external fun nativeExecuteTask(uiApp: Long, taskId: Int, assets: AssetManager)
    private external fun nativePostTask(uiApp: Long, taskId: Int)
    private external fun nativeUIShow(uiApp: Long)
    private external fun nativeUIHide(uiApp: Long)

    companion object {
        init {
            System.loadLibrary("skiaui")
        }

        private const val TAG = "SkiaUI"
    }

}