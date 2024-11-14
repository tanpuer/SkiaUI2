package com.temple.skiaui

import android.content.Context
import android.util.AttributeSet
import android.view.*

class HYSkiaSurfaceView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : SurfaceView(context, attrs, defStyleAttr), SurfaceHolder.Callback, Choreographer.FrameCallback{

    private lateinit var engine: HYSkiaEngine
    private var created = false

    init {
        holder.addCallback(this)
    }

    fun initEngine(type: Int) {
        engine = HYSkiaEngine(type)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        engine.createSurface(holder.surface)
        Choreographer.getInstance().postFrameCallback(this)
        created = true
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        engine.changeSurfaceSize(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        Choreographer.getInstance().removeFrameCallback(this)
        created = false
        engine.destroySurface()
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        engine.dispatchHYTouchEvent(event)
        return true
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (created) {
            engine.doFrame(frameTimeNanos / 1000000)
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    fun setRenderCallback(renderCallback: RenderCallback) {
        engine.renderCallback = renderCallback
    }

    fun onBackPressed() {
        engine.onBackPressed()
    }

    fun release() {
        engine.release()
    }

}