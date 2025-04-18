package com.temple.skiaui

import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.*

class HYSkiaTextureView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : TextureView(context, attrs), TextureView.SurfaceTextureListener, Choreographer.FrameCallback,
    ISkiaView {

    private lateinit var engine: HYSkiaEngine
    private var created = false

    init {
        surfaceTextureListener = this
    }

    override fun initEngine(type: Int) {
        engine = HYSkiaEngine(type, this)
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        engine.createSurface(Surface(surface))
        engine.changeSurfaceSize(width, height)
        created = true
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        Choreographer.getInstance().removeFrameCallback(this)
        created = false
        engine.destroySurface()
        return true
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {

    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {

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

    override fun setRenderCallback(renderCallback: RenderCallback) {
        engine.renderCallback = renderCallback
    }

    override fun onBackPressed() {
        engine.onBackPressed()
    }

    override fun release() {
        engine.release()
    }

    override fun onUIModeChange() {
        engine.onUIModeChange()
    }

}