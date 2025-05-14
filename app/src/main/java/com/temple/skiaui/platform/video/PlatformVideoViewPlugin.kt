package com.temple.skiaui.platform.video

import android.view.MotionEvent
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.SurfaceTextureBasePlugin

class PlatformVideoViewPlugin(engine: HYSkiaEngine, viewPtr: Long) :
    SurfaceTextureBasePlugin(engine, viewPtr, true), IVideoListener {

    private var exoPlayer: IVideoPlayer? = null
    private var customPlayer: IVideoPlayer? = null
    private var source: String = ""
    private var renderFirstFrame = false
    private var backgroundPlayback = false

    override fun onSurfaceCreated() {
        if (exoPlayer == null) {
            this.initializeReader()
        }
    }

    override fun onSurfaceChanged(width: Int, height: Int) {
    }

    override fun onSurfaceDestroyed() {
        if (backgroundPlayback) {
            return
        }
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
            exoPlayer?.setSource(source)
        }
    }

    private fun setCustomPlayer(player: IVideoPlayer) {
        pluginHandler.post {
            customPlayer = player
        }
    }

    private fun setBackgroundPlayback(enable: Boolean) {
        pluginHandler.post {
            backgroundPlayback = enable
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
            if (customPlayer != null) {
                return@post
            }
            exoPlayer?.release()
            exoPlayer = null
        }
        super.release()
    }

    override fun onShow() {
        super.onShow()
        pluginHandler.post {
            exoPlayer?.play()
        }
    }

    override fun onHide() {
        super.onHide()
        pluginHandler.post {
            if (backgroundPlayback) {
                return@post
            }
            exoPlayer?.pause()
        }
    }

}