package com.temple.skiaui.audio

import android.media.AudioFormat
import android.media.AudioTrack
import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.os.Handler
import android.os.HandlerThread
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
    private var sampleRate = 0
    private var channelCount = 1
    private var encoding = 0
    private var audioTracker: AudioTrack? = null

    init {
        decodeHandler.post {
            initializeReader()
        }
    }

    override fun release() {

    }

    override fun start() {
    }

    override fun pause() {
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
        val bufferSize = AudioTrack.getMinBufferSize(
            sampleRate,
            if (channelCount == 2) AudioFormat.CHANNEL_OUT_STEREO else AudioFormat.CHANNEL_OUT_MONO,
            1
        )
        val audioFormat = AudioFormat.Builder()
            .setEncoding(encoding)
            .setSampleRate(sampleRate)
            .build()
        audioTracker = AudioTrack.Builder()
            .setAudioFormat(audioFormat)
            .setBufferSizeInBytes(bufferSize)
            .setTransferMode(AudioTrack.MODE_STREAM)
            .build()
//        audioTracker?.write()
        audioTracker?.play()
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

    companion object {
        var INDEX = 0
    }
}