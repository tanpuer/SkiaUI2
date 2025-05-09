package com.temple.skiaui.platform

import android.graphics.Canvas
import android.graphics.SurfaceTexture
import android.graphics.SurfaceTexture.OnFrameAvailableListener
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R

abstract class PlatformTextureLayerBasePlugin(val engine: HYSkiaEngine, val viewPtr: Long) :
    ICanvasProvider, OnFrameAvailableListener {

    protected var width = 0

    protected var height = 0

    protected val mainHandler = engine.mainHandler

    protected var targetView: View? = null

    private var container: FrameLayout? = null

    private var skImagePtr: Long = 0L

    private var downTime: Long = 0L

    private var surfaceObj: SurfaceObj? = null

    @Volatile
    private var show: Boolean = true

    private val platformIndex = SurfaceObj.INDEX++

    private var index = "platform:${platformIndex}"

    @Volatile
    private var released = false

    private val createListener = fun(it: Boolean) {
        show = it
        if (!it) {
            reAttachSurfaceTexture(width, height)
        } else {
            engine.postToSkiaGL {
                surfaceObj?.surfaceTexture?.detachFromGLContext()
            }
            engine.postToSkiaUI {
                skImagePtr = 0L
            }
        }
    }

    init {
        mainHandler.post {
            container = (engine.view.parent as ViewGroup).findViewById(R.id.platformContainer)
            targetView = initPlatformView()
            container?.addView(targetView, ViewGroup.LayoutParams(width, height))
            engine.addSkiaSurfaceListener(index, createListener)
        }
    }

    abstract fun initPlatformView(): View

    abstract fun destroyPlatformView()

    fun getSkImage(): Long {
        return skImagePtr
    }

    fun release() {
        released = true
        mainHandler.post {
            engine.removeSurfaceListener(index)
            container?.removeView(targetView)
            destroyPlatformView()
            targetView = null
            surfaceObj?.release()
        }
        deleteSkImage(skImagePtr)
    }

    open fun reAttachSurfaceTexture(width: Int = this.width, height: Int = this.height) {
        surfaceObj?.surfaceTexture?.let { surfaceTexture ->
            engine.attachSurfaceTexture(width, height, surfaceTexture) {
                if (skImagePtr > 0) {
                    deleteSkImage(skImagePtr)
                }
                skImagePtr = it
            }
            onFrameAvailable(surfaceTexture)
        }
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture?) {
        if (this.released) {
            return
        }
        engine.postToSkiaGL {
            if (!this.show || this.released) {
                return@postToSkiaGL
            }
            surfaceObj?.surfaceTexture?.let {
                engine.updateTexImage(it, skImagePtr)
            }
        }
        engine.postToSkiaUI {
            if (!this.show || this.released) {
                return@postToSkiaUI
            }
            engine.markDirty(viewPtr)
        }
    }

    private fun sendTouchEvent(type: Int, x: Float, y: Float) {
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
            targetView?.onTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    private fun onSizeChange(width: Int, height: Int) {
        mainHandler.post {
            this.width = width
            this.height = height
            if (surfaceObj == null) {
                createSurface(width, height)
            } else if (surfaceObj?.width != width || surfaceObj?.height != height) {
                surfaceObj?.setDefaultBufferSize(width, height)
            }
            (targetView?.layoutParams as? FrameLayout.LayoutParams)?.apply {
                this.width = width
                this.height = height
                targetView?.requestLayout()
            }
        }
    }

    private fun createSurface(width: Int, height: Int) {
        if (!show) {
            return
        }
        if (surfaceObj == null) {
            surfaceObj = SurfaceObj().apply {
                this.surfaceTexture = SurfaceTexture(platformIndex).apply { detachFromGLContext() }
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


    override fun lockCanvas(originCanvas: Canvas): Canvas? {
        return surfaceObj?.surface?.lockHardwareCanvas()
    }

    override fun unLockCanvas(canvas: Canvas) {
        surfaceObj?.surface?.unlockCanvasAndPost(canvas)
    }

    override fun getSurface(): Surface? {
        return surfaceObj?.surface
    }

    private fun deleteSkImage(ptr: Long) {
        engine.deleteSkImage(ptr)
    }

}
