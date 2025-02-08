package com.temple.skiaui

import android.content.res.AssetManager
import android.graphics.SurfaceTexture
import android.hardware.HardwareBuffer
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.view.MotionEvent
import android.view.Surface
import android.view.View
import android.view.ViewConfiguration
import androidx.core.math.MathUtils.clamp
import com.temple.skiaui.plugin.PluginManager
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

class HYSkiaEngine(private val exampleType: Int, val view: View) {

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
    private val createListeners = mutableMapOf<String, (enable: Boolean) -> Unit>()
    private val executors = Executors.newFixedThreadPool(2)
    private var createdFlag = false

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
        createdFlag = true
    }

    fun changeSurfaceSize(width: Int, height: Int) {
        skiaGLHandler.post {
            nativeGLChanged(glApp, width, height, System.currentTimeMillis() / 1000)
        }
        skiaUIHandler.post {
            nativeUIChanged(uiApp, width, height, System.currentTimeMillis() / 1000)
            pic.set(nativeUIDoFrame(uiApp, System.currentTimeMillis() - start))
        }
        if (createdFlag) {
            createdFlag = false
            createListeners.forEach {
                it.value.invoke(true)
            }
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

    fun addSkiaSurfaceListener(key: String, callback: (enable: Boolean) -> Unit) {
        createListeners[key] = callback
    }

    fun removeSurfaceListener(key: String) {
        createListeners.remove(key)
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
            if (nextPic == 0L) {
                return@post
            }
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
                    val velocityX = (lastTouch.x - firstTouch.x) * 1000 / dt
                    val velocityY = (lastTouch.y - firstTouch.y) * 1000 / dt
                    val clampedVelocityX = clamp(velocityX, -MAX_FLING_VELOCITY, MAX_FLING_VELOCITY)
                    val clampedVelocityY = clamp(velocityY, -MAX_FLING_VELOCITY, MAX_FLING_VELOCITY)
                    nativeSetVelocity(
                        uiApp, clampedVelocityX, clampedVelocityY
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

    fun postToSkiaGL(runnable: Runnable) {
        skiaGLHandler.post(runnable)
    }

    fun postToSkiaUIDelay(runnable: Runnable, delay: Long) {
        skiaGLHandler.postDelayed(runnable, delay)
    }

    fun registerJetpackCompose() {
        nativeRegisterJetpackCompose()
    }

    fun setFocus(inputView: Long, focus: Boolean) {
        nativeSetFocus(uiApp, inputView, focus)
    }

    fun setTimeout(id: Long, delay: Long) {
        skiaUIHandler.postDelayed({
            nativePerformTimeout(uiApp, id)
        }, delay)
    }

    fun webViewProgressChange(webView: Long, progress: Int) {
        nativeWebViewProgressChange(webView, progress)
    }

    fun attachSurfaceTexture(
        width: Int,
        height: Int,
        surfaceTexture: SurfaceTexture,
        callback: (skImagePtr: Long) -> Unit
    ) {
        skiaGLHandler.post {
            val skImagePtr = nativeAttachSurfaceTexture(glApp, width, height, surfaceTexture)
            skiaUIHandler.post {
                callback(skImagePtr)
            }
        }
    }

    fun updateTexImage(surfaceTexture: SurfaceTexture, skImagePtr: Long) {
        nativeUpdateTexImage(glApp, surfaceTexture, skImagePtr)
    }

    fun markDirty(viewPtr: Long) {
        nativeMarkDirty(viewPtr)
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
    private external fun nativeAttachSurfaceTexture(
        glApp: Long,
        width: Int,
        height: Int,
        surfaceTexture: SurfaceTexture
    ): Long

    private external fun nativeUpdateTexImage(
        glApp: Long,
        surfaceTexture: SurfaceTexture,
        skImagePtr: Long
    )


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
    private external fun nativeSetFocus(uiApp: Long, inputView: Long, focus: Boolean)
    private external fun nativePerformTimeout(uiApp: Long, id: Long)

    private external fun nativeWebViewProgressChange(webView: Long, progress: Int)
    private external fun nativeMarkDirty(viewPtr: Long)

    companion object {
        init {
            System.loadLibrary("skiaui")
        }

        private const val TAG = "SkiaUI"
        private val MAX_FLING_VELOCITY =
            ViewConfiguration.get(HYSkiaUIApp.getInstance()).scaledMaximumFlingVelocity.toFloat()
    }

}