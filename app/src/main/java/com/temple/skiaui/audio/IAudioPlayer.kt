package com.temple.skiaui.audio

import android.Manifest
import android.content.pm.PackageManager
import android.media.audiofx.Visualizer
import android.media.audiofx.Visualizer.OnDataCaptureListener
import androidx.core.content.ContextCompat
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import kotlin.math.abs
import kotlin.math.hypot

/**
 * for exoplayer, for AudioTracker, for OpenSLES, for AAudio
 */
abstract class IAudioPlayer(
    protected val assetsPath: String,
    protected val engine: HYSkiaEngine
) {

    protected val count = 60

    /**
     * set/get in ui-thread
     */
    protected var fftData: FloatArray = floatArrayOf()

    protected var sessionId = -1

    protected var visualizer: Visualizer? = null

    @Volatile
    protected var released = false

    @Volatile
    protected var paused = false

    init {
        if (ContextCompat.checkSelfPermission(
                HYSkiaUIApp.getInstance(),
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            throw RuntimeException("need record_audio permission to use Visualizer API!")
        }
    }

    fun getFFTData(): FloatArray {
        return fftData
    }

    open fun release() {
        released = true
        visualizer?.setEnabled(false)
        visualizer?.release()
    }

    abstract fun start()

    abstract fun pause()

    abstract fun seek(mills: Long)

    abstract fun getCurrentPosition(): Long

    abstract fun getDuration(): Long

    abstract fun isPlaying(): Boolean

    protected fun createVisualizer(audioSessionId: Int) {
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
                    this@IAudioPlayer.fftData = model
                }
            }
        }, Visualizer.getMaxCaptureRate() / 2, false, true)
        visualizer?.setEnabled(true)
    }
}