package com.temple.skiaui.audio

import android.Manifest
import android.content.pm.PackageManager
import android.media.audiofx.Visualizer
import android.media.audiofx.Visualizer.OnDataCaptureListener
import android.net.Uri
import androidx.annotation.OptIn
import androidx.core.content.ContextCompat
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
import kotlin.math.abs
import kotlin.math.hypot

@OptIn(UnstableApi::class)
class HYSkiaAudioPlayer
    (
    assetsPath: String,
    engine: HYSkiaEngine
) : IAudioPlayer(assetsPath, engine) {
    private var exoPlayer: ExoPlayer = ExoPlayer.Builder(HYSkiaUIApp.getInstance()).build()

    init {
        if (ContextCompat.checkSelfPermission(
                HYSkiaUIApp.getInstance(),
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            throw RuntimeException("need record_audio permission to use Visualizer API!")
        }
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
        exoPlayer.stop();
        exoPlayer.release();
        visualizer?.setEnabled(false)
        visualizer?.release()
    }

    override fun start() {
        exoPlayer.playWhenReady = true
    }

    override fun pause() {
        exoPlayer.playWhenReady = false
    }

}