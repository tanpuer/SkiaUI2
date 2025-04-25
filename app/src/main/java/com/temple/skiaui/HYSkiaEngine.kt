package com.temple.skiaui

import android.content.Context
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.SurfaceTexture
import android.hardware.HardwareBuffer
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.view.MotionEvent
import android.view.Surface
import android.view.View
import android.view.ViewConfiguration
import androidx.annotation.MainThread
import androidx.core.math.MathUtils.clamp
import com.temple.skiaui.cache.PersistentCache
import com.temple.skiaui.compose.runtime.HYComposeSDK
import com.temple.skiaui.plugin.PluginManager
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

class HYSkiaEngine(private val developmentType: Int, val view: View) {

    /**
     * 执行UI逻辑
     */
    private val skiaUIHandlerThread: HandlerThread =
        HandlerThread("skia-ui", Thread.MAX_PRIORITY).apply {
            start()
        }
    private val skiaUIHandler =
        if (developmentType == DEVELOPMENT_COMPOSE) Handler(Looper.getMainLooper()) else Handler(
            skiaUIHandlerThread.looper
        )

    /**
     * 执行渲染逻辑
     */
    private val skiaGLHandlerThread: HandlerThread =
        HandlerThread("skia-gl", Thread.MAX_PRIORITY).apply {
            start()
        }
    private val skiaGLHandler = Handler(skiaGLHandlerThread.looper)

    private var pic = AtomicLong(0)
    private var picIsNull = AtomicBoolean(false)

    private val refreshRate = HYSkiaUIApp.getInstance().getFrameRate()
    private var frameCount = 0
    private var drawCount = AtomicInteger(0)
    private var renderCount = AtomicInteger(0)
    var renderCallback: RenderCallback? = null
    private val start = System.currentTimeMillis()
    private val pluginManager = PluginManager()
    private var glApp = 0L
    private var uiApp = 0L
    private val skImageList = mutableListOf<Long>()
    private val createListeners = mutableMapOf<String, (enable: Boolean) -> Unit>()
    private val sizeChangeListeners = mutableMapOf<String, (width: Int, height: Int) -> Unit>()
    private val executors = Executors.newFixedThreadPool(3)

    data class Velocity(val x: Float, val y: Float)

    private val touchPoints = mutableListOf<Velocity>()
    private val touchTimes = mutableListOf<Long>()

    init {
        skiaGLHandler.post {
            glApp = nativeGLInit()
        }
        skiaUIHandler.post {
            uiApp = nativeUIInit(HYSkiaUIApp.getInstance().assets, developmentType)
            nativeSetPlugins(uiApp, pluginManager)
        }
        PersistentCache.preload(executors)
    }

    @MainThread
    fun createSurface(surface: Surface) {
        pic.set(0L)
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

    @MainThread
    fun changeSurfaceSize(width: Int, height: Int) {
        skiaGLHandler.post {
            nativeGLChanged(glApp, width, height, System.currentTimeMillis() / 1000)
        }
        skiaUIHandler.post {
            nativeUIChanged(uiApp, width, height, System.currentTimeMillis() / 1000)
            pic.set(nativeUIDoFrame(uiApp, System.currentTimeMillis() - start))
        }
        sizeChangeListeners.forEach { (_, callback) ->
            callback.invoke(width, height)
        }
    }

    @MainThread
    fun destroySurface() {
        createListeners.forEach {
            it.value.invoke(false)
        }
        skiaUIHandler.post {
            nativeUIHide(uiApp)
        }
        skiaGLHandler.post {
            nativeGLDestroyed(glApp)
        }
    }

    @MainThread
    fun addSkiaSurfaceListener(key: String, callback: (enable: Boolean) -> Unit) {
        createListeners[key] = callback
    }

    @MainThread
    fun removeSurfaceListener(key: String) {
        createListeners.remove(key)
    }

    @MainThread
    fun addSizeChangeListener(key: String, callback: (width: Int, height: Int) -> Unit) {
        sizeChangeListeners[key] = callback
    }

    @MainThread
    fun removeSizeChangeListener(key: String) {
        sizeChangeListeners.remove(key)
    }

    @MainThread
    fun doFrame(time: Long) {
        frameCount++
        if (frameCount == refreshRate) {
            renderCallback?.updateFps(drawCount.get(), renderCount.get())
            drawCount.set(0)
            renderCount.set(0)
            frameCount = 0
        }
        skiaUIHandler.post {
            val nextPic = nativeUIDoFrame(uiApp, System.currentTimeMillis() - start)
            if (nextPic == 0L) {
                return@post
            }
            val prePic = pic.getAndSet(nextPic)
            if (picIsNull.compareAndSet(true, false)) {
                performGLDraw(time)
            }
            if (prePic != 0L) {
                nativeDeleteSkPicture(uiApp, prePic)
            }
            drawCount.addAndGet(1)
        }
        performGLDraw(time)
    }

    private fun performGLDraw(time: Long) {
        skiaGLHandler.post {
//            val measureStart = System.currentTimeMillis()
            val currPic = pic.getAndSet(0L)
            if (currPic == 0L) {
                picIsNull.set(true)
                return@post
            }
            nativeGLDoFrame(glApp, currPic, time)
//            val costTime = System.currentTimeMillis() - measureStart
//            Log.d(TAG, "GL cost: $costTime")
            renderCount.addAndGet(1)
            skImageList.forEach {
                nativeDeleteSkImage(glApp, it)
            }
            skImageList.clear()
        }
    }

    @MainThread
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
            skiaGLHandler.post {
                skiaGLHandlerThread.quitSafely()
            }
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
        hardwareBuffer: HardwareBuffer, callback: (skImagePtr: Long) -> Unit
    ) {
        skiaGLHandler.post {
            val skImagePtr = nativeGLMakeHardwareBufferToSkImage(glApp, hardwareBuffer)
            skiaUIHandler.post {
                callback.invoke(skImagePtr)
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

    fun sendInspectMsg(msg: String, nativePtr: Long) {
        skiaUIHandler.post {
            nativeSendInspectMsg(msg, nativePtr)
        }
    }

    @MainThread
    fun getContext(): Context {
        return view.context
    }

    fun onUIModeChange() {
        if (developmentType == DEVELOPMENT_COMPOSE) {
            HYComposeSDK.onUIModeChange()
        }
    }

    fun updateAndroidBitmap(ref: Long, bitmap: Bitmap, index: Int, frameCount: Int) {
        nativeUpdateAndroidBitmap(uiApp, ref, bitmap, index, frameCount)
    }

    private external fun nativeGLInit(): Long
    private external fun nativeGLCreated(glApp: Long, surface: Surface)
    private external fun nativeGLChanged(glApp: Long, width: Int, height: Int, time: Long)
    private external fun nativeGLDestroyed(glApp: Long)
    private external fun nativeGLDoFrame(glApp: Long, pic: Long, time: Long)
    private external fun nativeGLMakeHardwareBufferToSkImage(
        glApp: Long, hardwareBuffer: HardwareBuffer
    ): Long

    private external fun nativeDeleteSkImage(glApp: Long, skImagePtr: Long)
    private external fun nativeAttachSurfaceTexture(
        glApp: Long, width: Int, height: Int, surfaceTexture: SurfaceTexture
    ): Long

    private external fun nativeUpdateTexImage(
        glApp: Long, surfaceTexture: SurfaceTexture, skImagePtr: Long
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
    private external fun nativePerformTimeout(uiApp: Long, id: Long)
    private external fun nativeUpdateAndroidBitmap(
        uiApp: Long,
        ref: Long,
        bitmap: Bitmap,
        index: Int,
        frameCount: Int
    )

    private external fun nativeWebViewProgressChange(webView: Long, progress: Int)
    private external fun nativeMarkDirty(viewPtr: Long)
    private external fun nativeSendInspectMsg(msg: String, nativePtr: Long)

    companion object {
        init {
            System.loadLibrary("skiaui")
        }

        private const val TAG = "SkiaUIEngine"
        private val MAX_FLING_VELOCITY =
            ViewConfiguration.get(HYSkiaUIApp.getInstance()).scaledMaximumFlingVelocity.toFloat()
        const val DEVELOPMENT_CPP = 0
        const val DEVELOPMENT_JS = 1
        const val DEVELOPMENT_COMPOSE = 2
        const val DEVELOPMENT_REACT = 3
        const val DEVELOPMENT_VUE = 4
    }

}