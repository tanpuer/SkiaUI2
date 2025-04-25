package com.temple.skiaui.platform.video

import android.view.MotionEvent
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.SurfaceTextureBasePlugin

class PlatformVideoViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, viewPtr: Long) :
    SurfaceTextureBasePlugin(engine, width, height, viewPtr, true) {

    private var exoPlayer: ExoPlayerImpl? = null
    private var assetsPath: String = ""
    private var currentPosition: Long = 0L
    private var renderFirstFrame = false

    override fun onSurfaceCreated() {
    }

    override fun onSurfaceChanged(width: Int, height: Int) {
    }

    override fun onSurfaceDestroyed() {
        exoPlayer?.pause()
    }

    override fun dispatchTouchEvent(touchEvent: MotionEvent) {

    }

    override fun drawOneFrame(frameTimeNanos: Long) {

    }

    override fun type(): String = "ExoPlayerView"

    fun setSource(assetsPath: String) {
        renderFirstFrame = false
        pluginHandler.post {
            this.assetsPath = assetsPath
            createSurface()
            this.initializeReader()
        }
    }

    private fun initializeReader() {
        if (exoPlayer != null) {
            return
        }
        exoPlayer = ExoPlayerImpl()
        exoPlayer?.setAssetsSource(assetsPath)
        exoPlayer?.setVideoSurface(surfaceObj?.surface)
        exoPlayer?.setRepeat(true)
        exoPlayer?.prepare()
        exoPlayer?.play()
        exoPlayer?.setVideoListener(object : IVideoListener {
            override fun onRenderedFirstFrame() {
                engine.postToSkiaUI {
                    renderFirstFrame = true
                }
            }
        })
    }

    override fun release() {
        pluginHandler.post {
            exoPlayer?.stop()
            exoPlayer?.release()
            exoPlayer = null
        }
        super.release()
    }

    override fun onShow() {
        super.onShow()
        pluginHandler.post {
            val exoplayerIsNull = exoPlayer == null
            if (exoplayerIsNull) {
                initializeReader()
            }
            if (surfaceObj?.surface == null) {
                createSurface()
                exoPlayer?.setVideoSurface(surfaceObj?.surface)
            }
            exoPlayer?.play()
            if (exoplayerIsNull) {
                if (currentPosition != 0L) {
                    exoPlayer?.seekTo(currentPosition)
                    currentPosition = 0L
                }
            } else {
                exoPlayer?.seekTo(exoPlayer?.getCurrentPosition() ?: 0L)
            }
        }
    }

    override fun onHide() {
        super.onHide()
        pluginHandler.post {
            exoPlayer?.pause()
        }
    }

}