package com.temple.skiaui.platform

import android.graphics.Canvas
import android.graphics.SurfaceTexture
import android.graphics.SurfaceTexture.OnFrameAvailableListener
import android.util.Log
import android.view.InputDevice
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

    protected var container: FrameLayout? = null

    protected var skImagePtr: Long = 0L

    protected var downTime: Long = 0L

    protected var surfaceObj: SurfaceObj? = null

    @Volatile
    protected var show: Boolean = true

    protected val platformIndex = SurfaceObj.INDEX++

    protected var index = "platform:${platformIndex}"

    @Volatile
    protected var released = false

    @Volatile
    protected var firstFrameFlag = false

    private val createListener = fun(it: Boolean) {
        show = it
        if (it) {
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
            engine.addSkiaSurfaceListener(index, createListener)
            createSurface(1, 1)
            width = 1
            height = 1
            initView(width, height)
        }
    }

    abstract fun initPlatformView(): View

    abstract fun destroyPlatformView()

    fun getSkImage(): Long {
        return if (firstFrameFlag) skImagePtr else 0
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
                if (!firstFrameFlag) {
                    firstFrameFlag = true
                }
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
            motionEvent.source = InputDevice.SOURCE_TOUCHSCREEN
            Log.d("PlatformBasePlugin", "touch-event: $type $x $y")
            targetView?.onTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    open fun onSizeChange(left: Int, top: Int, width: Int, height: Int) {
        mainHandler.post {
            this.width = width
            this.height = height
            if (surfaceObj == null) {
                createSurface(width, height)
            } else if (surfaceObj?.width != width || surfaceObj?.height != height) {
                surfaceObj?.setDefaultBufferSize(width, height)
            }
            targetView?.layoutParams = (targetView?.layoutParams as? FrameLayout.LayoutParams)?.apply {
                this.width = width
                this.height = height
                this.leftMargin = left
                this.topMargin = top
            }
            targetView?.visibility = View.VISIBLE
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

    private fun setBackgroundColor(color: Int) {
        mainHandler.post {
            targetView?.setBackgroundColor(color)
        }
    }

    //Android views should be added to parent after SurfaceTexture has been crated and reattached to OESTexture
    private fun initView(width: Int, height: Int) {
        container = (engine.view.parent as ViewGroup).findViewById(R.id.platformContainer)
        targetView = initPlatformView()
        container?.addView(targetView, ViewGroup.LayoutParams(width, height))
        targetView?.visibility = View.INVISIBLE
    }

}
