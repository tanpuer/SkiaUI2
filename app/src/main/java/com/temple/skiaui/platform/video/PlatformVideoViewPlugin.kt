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

    override fun skiaSurfaceCreated() {
        pluginHandler.post {
            if (show && skiaShow) {
                createSurface()
                exoPlayer?.setVideoSurface(surfaceObj?.surface)
                exoPlayer?.play()
            }
        }
    }

    override fun skiaSurfaceDestroyed() {
        pluginHandler.post {
            exoPlayer?.setVideoSurface(null)
            exoPlayer?.pause()
        }
    }

    override fun dispatchTouchEvent(touchEvent: MotionEvent) {

    }

    override fun drawOneFrame(frameTimeNanos: Long) {

    }

    fun setSource(assetsPath: String) {
        pluginHandler.post {
            this.assetsPath = assetsPath
            createSurface()
            this.initializeReader()
        }
    }

    @OptIn(UnstableApi::class)
    private fun initializeReader() {
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
            if (surfaceObj?.surface == null) {
                createSurface()
                exoPlayer?.setVideoSurface(surfaceObj?.surface)
            }
            exoPlayer?.play()
        }
    }

    override fun onHide() {
        super.onHide()
        pluginHandler.post {
            exoPlayer?.pause()
        }
    }

}