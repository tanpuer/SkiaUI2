package com.temple.skiaui.audio

import android.net.Uri
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

@OptIn(UnstableApi::class)
class HYSkiaAudioPlayer
    (
    assetsPath: String,
    engine: HYSkiaEngine
) : IAudioPlayer(assetsPath, engine) {
    private var exoPlayer: ExoPlayer = ExoPlayer.Builder(HYSkiaUIApp.getInstance()).build()

    init {
        val dataSourceFactory = DataSource.Factory { AssetDataSource(HYSkiaUIApp.getInstance()) }
        val uri = Uri.parse("asset:///$assetsPath")
        val mediaSource: MediaSource = ProgressiveMediaSource.Factory(dataSourceFactory)
            .createMediaSource(MediaItem.fromUri(uri))
        exoPlayer.setMediaSource(mediaSource)
        exoPlayer.setVideoSurface(null)
        exoPlayer.repeatMode = Player.REPEAT_MODE_ALL
        exoPlayer.addListener(object : Player.Listener {

            override fun onPlaybackStateChanged(playbackState: Int) {
                super.onPlaybackStateChanged(playbackState)
                if (playbackState == Player.STATE_READY) {
                    createVisualizer(exoPlayer.audioSessionId)
                }
            }

        })
        exoPlayer.prepare()
        exoPlayer.playWhenReady = true
    }

    override fun release() {
        super.release()
        exoPlayer.stop();
        exoPlayer.release();
    }

    override fun start() {
        exoPlayer.playWhenReady = true
    }

    override fun pause() {
        exoPlayer.playWhenReady = false
    }

    override fun seek(mills: Long) {
        exoPlayer.seekTo(mills)
    }

    override fun getCurrentPosition(): Long {
        return exoPlayer.currentPosition
    }

    override fun getDuration(): Long {
        return exoPlayer.duration
    }

    override fun isPlaying(): Boolean {
        return exoPlayer.playWhenReady
    }

}