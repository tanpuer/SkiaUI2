package com.temple.skiaui.platform

import android.graphics.SurfaceTexture
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import android.view.Choreographer
import android.view.MotionEvent
import android.view.Surface
import com.temple.skiaui.HYSkiaEngine

abstract class SurfaceTextureBasePlugin(
    val engine: HYSkiaEngine,
    val width: Int,
    val height: Int,
    val viewPtr: Long,
    private val inMainThread: Boolean = false
) : Choreographer.FrameCallback, SurfaceTexture.OnFrameAvailableListener, HYSurfaceViewCallback {

    protected var skImagePtr: Long = 0L

    private var downTime: Long = 0L

    protected var surfaceObj: SurfaceObj? = null

    @Volatile
    protected var show: Boolean = true

    @Volatile
    protected var skiaShow: Boolean = true

    private val renderIndex = SurfaceObj.INDEX++

    private var index = "surface-texture:${renderIndex}"

    private val pluginThread = HandlerThread(index).apply {
        start()
    }

    protected val pluginHandler =
        if (inMainThread) engine.mainHandler else Handler(pluginThread.looper)

    protected val mainHandler = engine.mainHandler

    @Volatile
    protected var released = false

    private val createListener = fun(it: Boolean) {
        skiaShow = it
        val runnable = {
            if (!it) {
                skiaSurfaceDestroyed()
            } else {
                skiaSurfaceCreated()
            }
        }
        if (inMainThread) {
            runnable()
        } else {
            pluginHandler.post(runnable)
        }
    }

    init {
        mainHandler.post {
            engine.addSkiaSurfaceListener(index, createListener)
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    private fun skiaSurfaceCreated() {
        surfaceObj?.surfaceTexture?.let { surfaceTexture ->
            engine.attachSurfaceTexture(width, height, surfaceTexture) {
                skImagePtr = it
            }
        }
        onSurfaceCreated()
    }

    private fun skiaSurfaceDestroyed() {
        engine.postToSkiaGL {
            surfaceObj?.surfaceTexture?.detachFromGLContext()
        }
        engine.postToSkiaUI {
            skImagePtr = 0L
        }
        onSurfaceDestroyed()
    }

    open fun getSkImage(): Long {
        return skImagePtr
    }

    open fun release() {
        Choreographer.getInstance().removeFrameCallback(this)
        released = true
        pluginHandler.post {
            surfaceObj?.surfaceTexture?.setOnFrameAvailableListener(null)
            surfaceObj?.release()
            surfaceObj = null
        }
        mainHandler.post {
            engine.removeSurfaceListener(index)
        }
        deleteSkImage(skImagePtr)
        skImagePtr = 0L
        if (!inMainThread) {
            pluginThread.quitSafely()
        }
    }

    override fun onShow() {
        show = true
        Choreographer.getInstance().postFrameCallback(this)
        //TODO: pop to this page, onFrameAvailable sometimes will never be called
        onFrameAvailable(surfaceObj?.surfaceTexture)
    }

    override fun onHide() {
        show = false
        Choreographer.getInstance().removeFrameCallback(this)
    }

    private fun onSizeChange(width: Int, height: Int) {
        pluginHandler.post {
            if (surfaceObj?.width != width || surfaceObj?.height != height) {
                surfaceObj?.setDefaultBufferSize(width, height)
            }
        }
        onSurfaceChanged(width, height)
    }

    abstract fun type(): String

    abstract fun dispatchTouchEvent(touchEvent: MotionEvent)

    abstract fun drawOneFrame(frameTimeNanos: Long)

    override fun doFrame(frameTimeNanos: Long) {
        drawOneFrame(frameTimeNanos)
        Choreographer.getInstance().postFrameCallback(this)
    }

    fun sendTouchEvent(type: Int, x: Float, y: Float) {
        pluginHandler.post {
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
            dispatchTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    fun createSurface(width: Int = this.width, height: Int = this.height) {
        if (!show || !skiaShow) {
            return
        }
        if (surfaceObj == null) {
            surfaceObj = SurfaceObj().apply {
                this.surfaceTexture = SurfaceTexture(renderIndex).apply { detachFromGLContext() }
                this.surface = Surface(this.surfaceTexture)
                this.width = width
                this.height = height
            }
            surfaceObj?.setDefaultBufferSize(width, height)
            val surfaceTexture = surfaceObj?.surfaceTexture ?: return
            engine.attachSurfaceTexture(width, height, surfaceTexture) {
                skImagePtr = it
            }
            surfaceTexture.setOnFrameAvailableListener(this)
        }
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture?) {
        if (this.released) {
            return
        }
        engine.postToSkiaGL {
            if (!this.show || this.released || !skiaShow || surfaceTexture?.isReleased == true) {
                return@postToSkiaGL
            }
            surfaceObj?.surfaceTexture?.let {
                engine.updateTexImage(it, skImagePtr)
            }
        }
        engine.postToSkiaUI {
            if (!this.show || this.released || !skiaShow) {
                return@postToSkiaUI
            }
            engine.markDirty(viewPtr)
        }
    }

    fun deleteSkImage(ptr: Long) {
        engine.deleteSkImage(ptr)
    }

    companion object {
        private const val TAG = "SurfaceTextureBase"
    }
}