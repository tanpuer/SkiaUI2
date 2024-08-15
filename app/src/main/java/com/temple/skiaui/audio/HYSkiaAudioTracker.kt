package com.temple.skiaui.audio

import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import java.io.IOException

class HYSkiaAudioTracker(
    assetsPath: String,
    engine: HYSkiaEngine
) : IAudioPlayer(assetsPath, engine) {

    private lateinit var extractor: MediaExtractor
    private lateinit var decoder: MediaCodec

    private var duration = 0L

    init {

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
            val mime = format.getString(MediaFormat.KEY_MIME)
            decoder = MediaCodec.createDecoderByType(mime ?: "")
            decoder.configure(format, null, null, 0)
            decoder.start()
        } catch (e: IOException) {
            throw RuntimeException("Failed to initialize extractor or decoder", e)
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
}