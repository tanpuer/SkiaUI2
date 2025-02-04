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
    val viewPtr: Long
) : Choreographer.FrameCallback, SurfaceTexture.OnFrameAvailableListener {

    private var skImagePtr: Long = 0L

    private var downTime: Long = 0L

    protected var surfaceObj: SurfaceObj? = null

    @Volatile
    protected var show: Boolean = true

    @Volatile
    protected var skiaShow: Boolean = true

    private var index = "surface-texture:${INDEX++}"

    private val pluginThread = HandlerThread(index).apply {
        start()
    }

    protected val pluginHandler = Handler(pluginThread.looper)

    @Volatile
    protected var released = false

    private val createListener = fun(it: Boolean) {
        skiaShow = it
        pluginHandler.post {
            if (!it) {
                skiaSurfaceDestroyed()
                surfaceObj?.release()
                surfaceObj = null
            } else {
                skiaSurfaceCreated()
            }
        }
    }

    init {
        pluginHandler.post {
            engine.addSkiaSurfaceListener(index, createListener)
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    abstract fun skiaSurfaceCreated()

    abstract fun skiaSurfaceDestroyed()

    abstract fun type(): String

    fun getSkImage(): Long {
        return skImagePtr
    }

    open fun release() {
        Choreographer.getInstance().removeFrameCallback(this)
        released = true
        pluginHandler.post {
            surfaceObj?.surfaceTexture?.setOnFrameAvailableListener(null)
            engine.removeSurfaceListener(index)
            surfaceObj?.release()
        }
        deleteSkImage(skImagePtr)
        skImagePtr = 0L
        pluginHandler.post {
            pluginThread.quitSafely()
        }
    }

    open fun onShow() {
        show = true
        Choreographer.getInstance().postFrameCallback(this)
        //TODO: pop to this page, onFrameAvailable sometimes will never be called
        onFrameAvailable(surfaceObj?.surfaceTexture)
    }

    open fun onHide() {
        show = false
        Choreographer.getInstance().removeFrameCallback(this)
    }

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

    fun createSurface() {
        if (!show || !skiaShow) {
            return
        }
        if (surfaceObj == null) {
            surfaceObj = SurfaceObj().apply {
                this.surfaceTexture = SurfaceTexture(0).apply { detachFromGLContext() }
                this.surface = Surface(this.surfaceTexture)
                this.width = this@SurfaceTextureBasePlugin.width
                this.height = this@SurfaceTextureBasePlugin.height
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
            surfaceTexture?.updateTexImage()
        }
        engine.postToSkiaUI {
            if (!this.show || this.released || !skiaShow) {
                return@postToSkiaUI
            }
            engine.markDirty(viewPtr)
        }
    }

    private fun deleteSkImage(ptr: Long) {
        engine.deleteSkImage(ptr)
    }

    companion object {
        private var INDEX = 0
        private const val TAG = "SurfaceTextureBase"
    }
}