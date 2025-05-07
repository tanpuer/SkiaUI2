package com.temple.skiaui.audio

import android.media.AudioAttributes
import android.media.AudioFormat
import android.media.AudioTrack
import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import java.io.IOException

class HYSkiaAudioTracker(
    assetsPath: String,
    engine: HYSkiaEngine
) : IAudioPlayer(assetsPath, engine) {

    private lateinit var extractor: MediaExtractor
    private lateinit var decoder: MediaCodec

    private val threadName = "audio-decoder${INDEX++}"
    private val decodeThread = HandlerThread(threadName).apply {
        start()
    }
    private val decodeHandler = Handler(decodeThread.looper)

    private var duration = 0L
    private var sampleRate = 44100
    private var channelCount = 1
    private var encoding = -1
    private var audioTracker: AudioTrack? = null
    private var isEOS = false
    private var mainHandler = engine.mainHandler

    @Volatile
    private var presentationTimeUs = 0L
    private var markSeekFlag = false
    private var seekTimeMills = 0L

    init {
        decodeHandler.post {
            initializeReader()
            initAudioTracker()
            runDecode()
        }
    }

    override fun release() {
        super.release()
        audioTracker?.stop()
        audioTracker?.release()
        audioTracker = null
        decodeHandler.post {
            decoder.stop()
            decoder.release()
            extractor.release()
        }
    }

    override fun start() {
        paused = false
        audioTracker?.play()
        decodeHandler.post {
            runDecode()
        }
    }

    override fun pause() {
        paused = true
        audioTracker?.pause()
    }

    /**
     * 相对准确，Exoplayer的getCurrentPosition不准而且不实时
     */
    override fun getCurrentPosition(): Long {
        return ((audioTracker?.playbackHeadPosition ?: 0) * 1000L) / sampleRate + seekTimeMills
//        return presentationTimeUs / 1000
    }

    override fun getDuration(): Long {
        return duration
    }

    private fun initializeReader() {
        extractor = MediaExtractor()
        try {
            val afd = HYSkiaUIApp.getInstance().assets.openFd(assetsPath)
            extractor.setDataSource(afd)
            val trackIndex = selectAudioTrack(extractor)
            if (trackIndex < 0) {
                throw RuntimeException("No Audio track found in " + this.assetsPath)
            }
            extractor.selectTrack(trackIndex)
            val format = extractor.getTrackFormat(trackIndex)
            if (format.containsKey(MediaFormat.KEY_DURATION)) {
                duration = format.getLong(MediaFormat.KEY_DURATION) / 1000
            }
            if (format.containsKey(MediaFormat.KEY_SAMPLE_RATE)) {
                sampleRate = format.getInteger(MediaFormat.KEY_SAMPLE_RATE);
            }
            if (format.containsKey(MediaFormat.KEY_CHANNEL_COUNT)) {
                channelCount = format.getInteger(MediaFormat.KEY_CHANNEL_COUNT)
            }
            if (format.containsKey(MediaFormat.KEY_PCM_ENCODING)) {
                encoding = format.getInteger(MediaFormat.KEY_PCM_ENCODING)
            }
            val mime = format.getString(MediaFormat.KEY_MIME)
            decoder = MediaCodec.createDecoderByType(mime ?: "")
            decoder.configure(format, null, null, 0)
            decoder.start()
        } catch (e: IOException) {
            throw RuntimeException("Failed to initialize extractor or decoder", e)
        }
    }

    private fun initAudioTracker() {
        val channelConfig =
            if (channelCount == 2) AudioFormat.CHANNEL_OUT_STEREO else AudioFormat.CHANNEL_OUT_MONO
        val encoding = AudioFormat.ENCODING_PCM_16BIT
        val bufferSize = AudioTrack.getMinBufferSize(sampleRate, channelConfig, encoding)
        val audioFormat = AudioFormat.Builder()
            .setEncoding(encoding)
            .setSampleRate(sampleRate)
            .setChannelMask(channelConfig)
            .build()
        val attributes = AudioAttributes.Builder()
            .setUsage(AudioAttributes.USAGE_MEDIA)
            .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
            .build()
        audioTracker = AudioTrack.Builder()
            .setAudioAttributes(attributes)
            .setAudioFormat(audioFormat)
            .setTransferMode(AudioTrack.MODE_STREAM)
            .setBufferSizeInBytes(bufferSize)
            .setTransferMode(AudioTrack.MODE_STREAM)
            .build()
        audioTracker?.play()
        audioTracker?.audioSessionId?.let {
            engine.mainHandler.post {
                createVisualizer(it)
            }
        }
    }

    private fun runDecode() {
        while (!released && !paused && !isEOS) {
            startDecode()
        }
        if (isEOS) {
            isEOS = false
            seek(0)
            mainHandler.post {
                audioCallback?.onComplete()
            }
        }
    }

    private fun startDecode() {
        val info = MediaCodec.BufferInfo()
        val timeoutUs: Long = 10000

        val inputBufferId = decoder.dequeueInputBuffer(timeoutUs)
        if (inputBufferId >= 0) {
            val inputBuffer = decoder.getInputBuffer(inputBufferId) ?: return
            val sampleSize = extractor.readSampleData(inputBuffer, 0)
            if (sampleSize < 0) {
                decoder.queueInputBuffer(
                    inputBufferId, 0, 0, 0L, MediaCodec.BUFFER_FLAG_END_OF_STREAM
                )
            } else {
                presentationTimeUs = extractor.sampleTime
                if (markSeekFlag) {
                    markSeekFlag = false
                    seekTimeMills = extractor.sampleTime / 1000
                }
                decoder.queueInputBuffer(inputBufferId, 0, sampleSize, presentationTimeUs, 0)
                extractor.advance()
            }
        }

        val outputBufferId = decoder.dequeueOutputBuffer(info, timeoutUs)
        if (outputBufferId >= 0) {
            decoder.getOutputBuffer(outputBufferId)?.let {
                it.position(0)
                runCatching {
                    audioTracker?.write(it, it.remaining(), AudioTrack.WRITE_BLOCKING)
                }
            }
            decoder.releaseOutputBuffer(outputBufferId, false)
            if ((info.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                isEOS = true
            }
        }
    }

    private fun selectAudioTrack(extractor: MediaExtractor): Int {
        val numTracks = extractor.trackCount
        for (i in 0 until numTracks) {
            val format = extractor.getTrackFormat(i)
            val mime = format.getString(MediaFormat.KEY_MIME)
            if (mime?.startsWith("audio/") == true) {
                return i
            }
        }
        return -1
    }

    override fun seek(mills: Long) {
        paused = true
        decodeHandler.post {
            audioTracker?.pause()
            audioTracker?.flush()
            decoder.flush()
            markSeekFlag = true
            seekTimeMills = mills
            extractor.seekTo(mills * 1000, MediaExtractor.SEEK_TO_PREVIOUS_SYNC)
            paused = false
            audioTracker?.play()
            runDecode()
        }
    }

    override fun isPlaying(): Boolean {
        return audioTracker?.playState == AudioTrack.PLAYSTATE_PLAYING
    }

    companion object {
        var INDEX = 0
    }
}