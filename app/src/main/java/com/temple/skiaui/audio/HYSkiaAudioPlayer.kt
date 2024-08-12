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
    private val assetsPath: String,
    private val engine: HYSkiaEngine
) {
    private var exoPlayer: ExoPlayer = ExoPlayer.Builder(HYSkiaUIApp.getInstance()).build()

    private var visualizer: Visualizer? = null

    private val count = 60

    private var sessionId = -1

    /**
     * set/get in ui-thread
     */
    private var fftData: FloatArray = floatArrayOf()

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

    fun getFFTData(): FloatArray {
        return fftData
    }

    fun release() {
        exoPlayer.stop();
        exoPlayer.release();
        visualizer?.setEnabled(false)
        visualizer?.release()
    }

    fun start() {
        exoPlayer.playWhenReady = true
    }

    fun pause() {
        exoPlayer.playWhenReady = false
    }

    private fun createVisualizer(audioSessionId: Int) {
        if (sessionId == audioSessionId) {
            return
        }
        sessionId = audioSessionId
        visualizer?.setEnabled(false)
        visualizer?.release()
        visualizer = Visualizer(audioSessionId)
        visualizer?.setCaptureSize(Visualizer.getCaptureSizeRange()[1]);
        visualizer?.setDataCaptureListener(object : OnDataCaptureListener {
            override fun onWaveFormDataCapture(
                visualizer: Visualizer,
                bytes: ByteArray,
                samplingRate: Int
            ) {
                // ignore
            }

            override fun onFftDataCapture(
                visualizer: Visualizer,
                fft: ByteArray,
                samplingRate: Int
            ) {
                val model = FloatArray(fft.size / 2 + 1)
                model[0] = abs(fft[1].toFloat())
                var j = 1
                var i = 2
                while (i < count * 2) {
                    model[j] = hypot(fft[i].toFloat(), fft[i + 1].toFloat())
                    i += 2
                    j++
                    model[j] = abs(fft[j].toFloat())
                }
                engine.postToSkiaUI {
                    this@HYSkiaAudioPlayer.fftData = model
                }
            }
        }, Visualizer.getMaxCaptureRate() / 2, false, true)
        visualizer?.setEnabled(true)
    }
}