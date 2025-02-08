package com.temple.skiaui.platform

import android.graphics.Canvas
import android.graphics.SurfaceTexture
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R

abstract class PlatformTextureLayerBasePlugin(
    val engine: HYSkiaEngine,
    val width: Int,
    val height: Int,
    val viewPtr: Long
) : ICanvasProvider {

    protected val mainHandler = Handler(Looper.getMainLooper())

    protected var targetView: View? = null

    private var container: FrameLayout? = null

    private var skImagePtr: Long = 0L

    private var downTime: Long = 0L

    private var surfaceObj: SurfaceObj? = null

    @Volatile
    private var show: Boolean = true

    private var index = "platform:${INDEX++}"

    @Volatile
    private var released = false

    init {
        mainHandler.post {
            container = (engine.view.parent as ViewGroup).findViewById(R.id.platformContainer)
            targetView = initPlatformView()
            container?.addView(targetView, ViewGroup.LayoutParams(width, height))
            engine.addSkiaSurfaceListener(index, createListener)
        }
    }

    private val createListener = fun(it: Boolean) {
        show = it
        if (!it) {
            surfaceObj?.release()
            surfaceObj = null
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
            targetView?.onTouchEvent(motionEvent)
            motionEvent.recycle()
        }
    }

    override fun lockCanvas(originCanvas: Canvas): Canvas? {
        if (!show) {
            return null
        }
        if (surfaceObj == null) {
            surfaceObj = SurfaceObj().apply {
                this.surfaceTexture = SurfaceTexture(0).apply { detachFromGLContext() }
                this.surface = Surface(this.surfaceTexture)
                this.width = originCanvas.width
                this.height = originCanvas.height
            }
            surfaceObj?.setDefaultBufferSize(originCanvas.width, originCanvas.height)
            val surfaceTexture = surfaceObj?.surfaceTexture ?: return null
            engine.attachSurfaceTexture(originCanvas.width, originCanvas.height, surfaceTexture) {
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
        }
        if (surfaceObj?.width != originCanvas.width || surfaceObj?.height != originCanvas.height) {
            surfaceObj?.setDefaultBufferSize(originCanvas.width, originCanvas.height)
        }
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

    companion object {
        private var INDEX = 0
    }

}