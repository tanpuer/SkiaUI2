package com.temple.skiaui

import android.content.res.AssetManager
import android.hardware.HardwareBuffer
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.view.MotionEvent
import android.view.Surface
import com.temple.skiaui.plugin.PluginManager
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

class HYSkiaEngine(val exampleType: Int) {

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
    val createListeners = mutableMapOf<String, (enable: Boolean) -> Unit>()
    private val executors = Executors.newFixedThreadPool(2)

    data class Velocity(val x: Float, val y: Float)
    private val touchPoints = mutableListOf<Velocity>()
    private val touchTimes = mutableListOf<Long>()

    init {
        skiaGLHandler.post {
            glApp = nativeGLInit()
        }
        skiaUIHandler.post {
            uiApp = nativeUIInit(HYSkiaUIApp.getInstance().assets, exampleType)
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
        createListeners.forEach {
            it.value.invoke(true)
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
        createListeners.forEach {
            it.value.invoke(false)
        }
    }

    fun doFrame(time: Long) {
        frameCount++
        if (frameCount == refreshRate) {
            renderCallback?.updateFps(drawCount.get())
            drawCount.set(0)
            frameCount = 0
        }
        skiaUIHandler.post {
            val nextPic = nativeUIDoFrame(uiApp, System.currentTimeMillis() - start)
            val prePic = pic.getAndSet(nextPic)
            if (prePic != 0L) {
                nativeDeleteSkPicture(uiApp, prePic)
            }
            drawCount.set(drawCount.get() + 1)
        }
        skiaGLHandler.post {
            val currPic = pic.getAndSet(0L)
            if (currPic == 0L) {
                return@post
            }
            nativeGLDoFrame(glApp, currPic, time)
            skImageList.forEach {
                nativeDeleteSkImage(glApp, it)
            }
            skImageList.clear()
        }
    }

    fun dispatchHYTouchEvent(event: MotionEvent): Boolean {
        val x = event.x
        val y = event.y
        val action = event.action
        if (action == MotionEvent.ACTION_DOWN) {
            touchPoints.clear()
            touchTimes.clear()
        }
        if (touchPoints.size >= 5) {
            touchPoints.removeAt(0)
            touchTimes.removeAt(0)
        }
        touchPoints.add(Velocity(x, y))
        touchTimes.add(System.currentTimeMillis())
        if (action == MotionEvent.ACTION_UP) {
            if (touchPoints.size >= 2) {
                val firstTouch = touchPoints.first()
                val lastTouch = touchPoints.last()
                val firstTime = touchTimes.first()
                val lastTime = touchTimes.last()
                val dt = lastTime - firstTime
                if (dt > 0) {
                    nativeSetVelocity(
                        uiApp,
                        (lastTouch.x - firstTouch.x) * 1000 / dt,
                        (lastTouch.y - firstTouch.y) * 1000 / dt,
                    )
                }
            }
        }
        skiaUIHandler.post {
            nativeTouchEvent(uiApp, action, x, y)
        }
        return true
    }

    fun release() {
        skiaUIHandler.post {
            nativeRelease(uiApp, glApp)
            uiApp = 0L
            glApp = 0L
            executors.shutdown()
            skiaUIHandlerThread.quitSafely()
            skiaGLHandlerThread.quitSafely()
        }
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

    fun postToSkiaUI(runnable: Runnable) {
        skiaUIHandler.post(runnable)
    }

    fun postToSkiaUIDelay(runnable: Runnable, delay: Long) {
        skiaGLHandler.postDelayed(runnable, delay)
    }

    fun registerJetpackCompose() {
        nativeRegisterJetpackCompose()
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

    private external fun nativeUIInit(assets: AssetManager, exampleType: Int): Long
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
    private external fun nativeDeleteSkPicture(uiApp: Long, skPicture: Long)
    private external fun nativeRegisterJetpackCompose()

    companion object {
        init {
            System.loadLibrary("skiaui")
        }

        private const val TAG = "SkiaUI"
    }

}