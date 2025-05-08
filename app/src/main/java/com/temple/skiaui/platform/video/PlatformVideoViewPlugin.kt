package com.temple.skiaui.platform.video

import android.view.MotionEvent
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.SurfaceTextureBasePlugin

class PlatformVideoViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, viewPtr: Long) :
    SurfaceTextureBasePlugin(engine, width, height, viewPtr, true), IVideoListener {

    private var exoPlayer: IVideoPlayer? = null
    private var customPlayer: IVideoPlayer? = null
    private var source: String = ""
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

    private fun setSource(source: String) {
        renderFirstFrame = false
        pluginHandler.post {
            this.source = source
            createSurface()
            if (exoPlayer == null) {
                this.initializeReader()
            } else {
                exoPlayer?.setSource(source)
            }
        }
    }

    private fun setCustomPlayer(player: IVideoPlayer) {
        pluginHandler.post {
            customPlayer = player
        }
    }

    private fun initializeReader() {
        exoPlayer = customPlayer ?: ExoPlayerImpl()
        exoPlayer?.setSource(source)
        exoPlayer?.setVideoSurface(surfaceObj?.surface)
        exoPlayer?.setRepeat(true)
        exoPlayer?.prepare()
        exoPlayer?.play()
        exoPlayer?.setVideoListener(this)
    }

    override fun onRenderedFirstFrame() {
        engine.postToSkiaUI {
            renderFirstFrame = true
        }
    }

    override fun onVideoSizeChanged(videoWidth: Int, videoHeight: Int) {

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