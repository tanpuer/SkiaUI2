package com.temple.skiaui.platform.video

import android.util.Log
import android.view.Surface
import androidx.annotation.OptIn
import androidx.core.net.toUri
import androidx.media3.common.MediaItem
import androidx.media3.common.Player
import androidx.media3.common.VideoSize
import androidx.media3.common.util.UnstableApi
import androidx.media3.datasource.AssetDataSource
import androidx.media3.datasource.DataSource
import androidx.media3.datasource.DefaultDataSource
import androidx.media3.datasource.FileDataSource
import androidx.media3.exoplayer.ExoPlayer
import androidx.media3.exoplayer.source.MediaSource
import androidx.media3.exoplayer.source.ProgressiveMediaSource
import com.temple.skiaui.HYSkiaUIApp

open class ExoPlayerImpl : IVideoPlayer {

    private var exoPlayer: ExoPlayer? = ExoPlayer.Builder(HYSkiaUIApp.getInstance()).build()
    private var listener: IVideoListener? = null
    private var start = 0L
    private var source = ""

    init {
        exoPlayer?.addListener(object : Player.Listener {
            override fun onRenderedFirstFrame() {
                this@ExoPlayerImpl.onRenderedFirstFrame()
            }

            override fun onVideoSizeChanged(videoSize: VideoSize) {
                this@ExoPlayerImpl.onVideoSizeChanged(videoSize.width, videoSize.height)
            }

            override fun onPlaybackStateChanged(playbackState: Int) {
                if (playbackState == Player.STATE_ENDED) {
                    this@ExoPlayerImpl.onPlayEnd()
                }
            }
        })
    }

    @OptIn(UnstableApi::class)
    override fun setSource(source: String) {
        this.source = source
        start = System.currentTimeMillis()
        if (source.startsWith("file://")) {
            val dataSourceFactory =
                DataSource.Factory { FileDataSource() }
            val mediaSource: MediaSource = ProgressiveMediaSource.Factory(dataSourceFactory)
                .createMediaSource(MediaItem.fromUri(source))
            exoPlayer?.setMediaSource(mediaSource)
        } else if (source.startsWith("http://") || source.startsWith("https://")) {
            val dataSourceFactory = DefaultDataSource.Factory(HYSkiaUIApp.getInstance())
            val mediaSource = ProgressiveMediaSource.Factory(dataSourceFactory)
                .createMediaSource(MediaItem.fromUri(source))
            exoPlayer?.setMediaSource(mediaSource)
        } else {
            val dataSourceFactory =
                DataSource.Factory { AssetDataSource(HYSkiaUIApp.getInstance()) }
            val uri = "asset:///$source".toUri()
            val mediaSource: MediaSource = ProgressiveMediaSource.Factory(dataSourceFactory)
                .createMediaSource(MediaItem.fromUri(uri))
            exoPlayer?.setMediaSource(mediaSource)
        }
    }

    override fun setVideoSurface(surface: Surface?) {
        exoPlayer?.setVideoSurface(surface)
    }

    override fun setRepeat(flag: Boolean) {
        exoPlayer?.repeatMode = if (flag) Player.REPEAT_MODE_ALL else Player.REPEAT_MODE_OFF
    }

    override fun prepare() {
        exoPlayer?.prepare()
    }

    override fun play() {
        exoPlayer?.play()
    }

    override fun pause() {
        exoPlayer?.pause()
    }

    override fun stop() {
        exoPlayer?.stop()
    }

    override fun release() {
        exoPlayer?.release()
        exoPlayer = null
    }

    override fun seekTo(positionMs: Long) {
        exoPlayer?.seekTo(positionMs)
    }

    override fun getCurrentPosition(): Long {
        return exoPlayer?.currentPosition ?: 0L
    }

    override fun getDuration(): Long {
        return exoPlayer?.duration ?: 0L
    }

    override fun setVideoListener(listener: IVideoListener?) {
        this.listener = listener
    }

    override fun onRenderedFirstFrame() {
        Log.d(TAG, "first-frame: ${System.currentTimeMillis() - start}")
        listener?.onRenderedFirstFrame()
    }

    override fun onVideoSizeChanged(videoWidth: Int, videoHeight: Int) {
        Log.d(TAG, "video-width: ${videoWidth}, video-height:${videoHeight}")
        listener?.onVideoSizeChanged(videoWidth, videoHeight)
    }

    override fun onPlayEnd() {
        Log.d(TAG, "onPlayEnd: $source")
        listener?.onPlayEnd()
    }

    companion object {
        private const val TAG = "ExoPlayerImpl"
    }

}
