package com.temple.skiaui.compose.ui

import android.graphics.SurfaceTexture
import android.view.Surface
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.platform.SurfaceObj

class HYComposeTextureView(modifier: Modifier, val engine: HYSkiaEngine) : HYComposeView(modifier),
    SurfaceTexture.OnFrameAvailableListener {

    private var createdCallback: ((surface: Surface) -> Unit)? = null
    private var changedCallback: ((surface: Surface, width: Int, height: Int) -> Unit)? =
        null
    private var destroyedCallback: ((surface: Surface) -> Unit)? = null

    private var surfaceObj: SurfaceObj? = null

    private var width = 0
    private var height = 0
    private var skImagePtr = 0L

    init {
        engine.addSkiaSurfaceListener("TextureView-${INDEX++}") {
            if (it) {
                skiaSurfaceCreated()
            } else {
                skiaSurfaceDestroyed()
            }
        }
    }

    fun onSurfaceTextureCreated(callback: (surface: Surface) -> Unit) {
        createdCallback = callback
    }

    fun onSurfaceTextureChanged(callback: (surface: Surface, width: Int, height: Int) -> Unit) {
        changedCallback = callback
    }

    fun onSurfaceTextureDestroyed(callback: (surface: Surface) -> Unit) {
        destroyedCallback = callback
    }

    override fun clear() {
        val surface = surfaceObj?.surface ?: return
        destroyedCallback?.invoke(surface)
    }

    override fun getViewType(): String = "TextureView"

    ///////////////////////call from C++///////////////////////

    private fun getSkImage(): Long {
        return skImagePtr
    }

    private fun onShow() {

    }

    private fun onHide() {

    }

    private fun release() {

    }

    private fun sendTouchEvent(action: Int, x: Float, y: Float) {

    }

    private fun onSizeChange(width: Int, height: Int) {
        this.width = width
        this.height = height
        if (surfaceObj == null) {
            createSurface()
            val surface = surfaceObj?.surface ?: return
            createdCallback?.invoke(surface)
            changedCallback?.invoke(surface, width, height)
        } else if (surfaceObj?.width != width || surfaceObj?.height != height) {
            surfaceObj?.setDefaultBufferSize(width, height)
            val surface = surfaceObj?.surface ?: return
            changedCallback?.invoke(surface, width, height)
        }
    }

    ///////////////////////call from C++///////////////////////

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture?) {
        engine.postToSkiaGL {
            surfaceObj?.surfaceTexture?.let {
                engine.updateTexImage(it, skImagePtr)
            }
        }
        engine.postToSkiaUI {
            engine.markDirty(ref)
        }
    }

    private fun createSurface() {
        if (surfaceObj == null) {
            surfaceObj = SurfaceObj().apply {
                val renderIndex = SurfaceObj.INDEX++
                this.surfaceTexture = SurfaceTexture(renderIndex).apply { detachFromGLContext() }
                this.surface = Surface(this.surfaceTexture)
                this.width = this@HYComposeTextureView.width
                this.height = this@HYComposeTextureView.height
            }
            surfaceObj?.setDefaultBufferSize(width, height)
            val surfaceTexture = surfaceObj?.surfaceTexture ?: return
            engine.attachSurfaceTexture(width, height, surfaceTexture) {
                skImagePtr = it
            }
            surfaceTexture.setOnFrameAvailableListener(this)
        }
    }

    private fun reAttachSurfaceTexture(width: Int = this.width, height: Int = this.height) {
        surfaceObj?.surfaceTexture?.let { surfaceTexture ->
            engine.attachSurfaceTexture(width, height, surfaceTexture) {
                if (skImagePtr > 0) {
                    engine.deleteSkImage(skImagePtr)
                }
                skImagePtr = it
            }
            onFrameAvailable(surfaceTexture)
        }
    }

    private fun skiaSurfaceCreated() {
        if (surfaceObj == null && width > 0 && height > 0) {
            onSizeChange(width, height)
        }
        reAttachSurfaceTexture()
    }

    private fun skiaSurfaceDestroyed() {
        engine.postToSkiaGL {
            surfaceObj?.surfaceTexture?.detachFromGLContext()
        }
        engine.postToSkiaUI {
            skImagePtr = 0L
        }
        val surface = surfaceObj?.surface ?: return
        destroyedCallback?.invoke(surface)
    }

    companion object {
        private var INDEX = 1
    }

}