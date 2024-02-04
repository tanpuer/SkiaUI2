package com.temple.skiaui

import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.*

/**
 * 使用SurfaceView，渲染延迟感觉比TextureView要明显，同样都是监听vync信号，在doFrame回调里进行渲染，
 * 比如跟手这个直观的感觉，TextureView要比SurfaceView好。
 */
class HYSkiaTextureView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : TextureView(context, attrs), TextureView.SurfaceTextureListener {

    private val engine = HYSkiaEngine()

    init {
        surfaceTextureListener = this
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        engine.createSurface(Surface(surface))
        engine.changeSurfaceSize(width, height)
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        engine.destroySurface()
        return true
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {

    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {

    }

    override fun dispatchTouchEvent(event: MotionEvent): Boolean {
        return engine.dispatchHYTouchEvent(event)
    }

}