package com.temple.skiaui.platform

import android.graphics.SurfaceTexture
import android.os.Handler
import android.os.Looper
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
) : Choreographer.FrameCallback {

    protected val mainHandler = Handler(Looper.getMainLooper())

    private var skImagePtr: Long = 0L

    private var downTime: Long = 0L

    protected var surfaceObj: SurfaceObj? = null

    @Volatile
    private var show: Boolean = true

    private var index = "surface-texture:${INDEX++}"

    @Volatile
    private var released = false

    init {
        mainHandler.post {
            engine.createListeners[index] = createListener
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    private val createListener = fun(it: Boolean) {
        show = it
        if (!it) {
            skiaSurfaceDestroyed()
            surfaceObj?.release()
            surfaceObj = null
        } else {
            skiaSurfaceCreated()
        }
    }

    abstract fun skiaSurfaceCreated()

    abstract fun skiaSurfaceDestroyed()

    fun getSkImage(): Long {
        return skImagePtr
    }

    open fun release() {
        released = true
        mainHandler.post {
            engine.createListeners.remove(index)
            surfaceObj?.release()
        }
        deleteSkImage(skImagePtr)
    }

    open fun onShow() {
        show = true
        Choreographer.getInstance().postFrameCallback(this)
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
            dispatchTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    fun createSurface() {
        if (!show) {
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
            surfaceTexture.setOnFrameAvailableListener {
                if (this.released) {
                    return@setOnFrameAvailableListener
                }
                engine.postToSkiaGL {
                    if (!this.show || this.released) {
                        return@postToSkiaGL
                    }
                    surfaceObj?.surfaceTexture?.updateTexImage()
                }
                engine.postToSkiaUI {
                    if (!this.show || this.released) {
                        return@postToSkiaUI
                    }
                    engine.markDirty(viewPtr)
                }
            }
        }
    }

    private fun deleteSkImage(ptr: Long) {
        engine.deleteSkImage(ptr)
    }

    companion object {
        private var INDEX = 0
    }
}