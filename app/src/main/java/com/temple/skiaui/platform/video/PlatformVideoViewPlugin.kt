package com.temple.skiaui.platform.video

import android.net.Uri
import android.view.MotionEvent
import androidx.annotation.OptIn
import androidx.media3.common.MediaItem
import androidx.media3.common.Player
import androidx.media3.common.util.UnstableApi
import androidx.media3.datasource.AssetDataSource
import androidx.media3.datasource.DataSource
import androidx.media3.exoplayer.ExoPlayer
import androidx.media3.exoplayer.source.MediaSource
import androidx.media3.exoplayer.source.ProgressiveMediaSource
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.platform.SurfaceTextureBasePlugin

class PlatformVideoViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, viewPtr: Long) :
    SurfaceTextureBasePlugin(engine, width, height, viewPtr) {

    private var exoPlayer: ExoPlayer? = null
    private var assetsPath: String = ""
    private var currentPosition: Long = 0L
    private var renderFirstFrame = false

    override fun skiaSurfaceCreated() {
    }

    override fun skiaSurfaceDestroyed() {
        pluginHandler.post {
            if (exoPlayer == null) {
                return@post
            }
            currentPosition = exoPlayer?.currentPosition ?: 0L
            exoPlayer?.setVideoSurface(null)
            exoPlayer?.release()
            exoPlayer = null
        }
    }

    override fun dispatchTouchEvent(touchEvent: MotionEvent) {

    }

    override fun drawOneFrame(frameTimeNanos: Long) {

    }

    override fun getSkImage(): Long {
        return if (renderFirstFrame) skImagePtr else 0
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

    @OptIn(UnstableApi::class)
    private fun initializeReader() {
        if (exoPlayer != null) {
            return
        }
        exoPlayer = ExoPlayer.Builder(HYSkiaUIApp.getInstance()).build()
        val dataSourceFactory = DataSource.Factory { AssetDataSource(HYSkiaUIApp.getInstance()) }
        val uri = Uri.parse("asset:///$assetsPath")
        val mediaSource: MediaSource = ProgressiveMediaSource.Factory(dataSourceFactory)
            .createMediaSource(MediaItem.fromUri(uri))
        exoPlayer?.setMediaSource(mediaSource)
        exoPlayer?.setVideoSurface(surfaceObj?.surface)
        exoPlayer?.repeatMode = Player.REPEAT_MODE_ALL
        exoPlayer?.prepare()
        exoPlayer?.play()
        exoPlayer?.addListener(object : Player.Listener {
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
                exoPlayer?.seekTo(exoPlayer?.currentPosition ?: 0L)
            }
        }
    }

    override fun onHide() {
        super.onHide()
        skiaSurfaceDestroyed()
    }

}