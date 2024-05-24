package com.temple.skiaui.video

import android.content.Context
import android.graphics.ImageFormat
import android.hardware.HardwareBuffer
import android.media.ImageReader
import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.net.Uri
import android.os.Build
import android.view.Surface
import java.io.IOException

class HYSkVideo internal constructor(private val context: Context, localUri: String) {
    private val uri: Uri = Uri.parse(localUri)

    private lateinit var extractor: MediaExtractor
    private lateinit var decoder: MediaCodec
    private var imageReader: ImageReader? = null
    private var outputSurface: Surface? = null

    var duration: Double = 0.0
        private set

    var frameRate: Double = 0.0
        private set

    init {
        this.initializeReader()
    }

    private fun initializeReader() {
        extractor = MediaExtractor()
        try {
            extractor.setDataSource(context, this.uri, null)
            val trackIndex = selectVideoTrack(extractor)
            if (trackIndex < 0) {
                throw RuntimeException("No video track found in " + this.uri)
            }
            extractor.selectTrack(trackIndex)
            val format = extractor.getTrackFormat(trackIndex)
            // Retrieve and store video properties
            if (format.containsKey(MediaFormat.KEY_DURATION)) {
                duration = (format.getLong(MediaFormat.KEY_DURATION) / 1000).toDouble()
            }
            if (format.containsKey(MediaFormat.KEY_FRAME_RATE)) {
                frameRate = format.getInteger(MediaFormat.KEY_FRAME_RATE).toDouble()
            }
            val width = format.getInteger(MediaFormat.KEY_WIDTH)
            val height = format.getInteger(MediaFormat.KEY_HEIGHT)
            imageReader = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                ImageReader.newInstance(
                    width, height, ImageFormat.PRIVATE, 2, HardwareBuffer.USAGE_GPU_SAMPLED_IMAGE
                )
            } else {
                ImageReader.newInstance(
                    width, height, ImageFormat.PRIVATE, 2
                )
            }
            outputSurface = imageReader?.surface

            // Create a decoder for the format
            val mime = format.getString(MediaFormat.KEY_MIME)
            decoder = MediaCodec.createDecoderByType(mime ?: "")

            // Note: Use an output Surface for rendering if necessary, otherwise handle buffers
            decoder.configure(format, outputSurface, null, 0)
            decoder.start()
        } catch (e: IOException) {
            throw RuntimeException("Failed to initialize extractor or decoder", e)
        }
    }

    fun nextImage(): HardwareBuffer? {
        if (!decoderOutputAvailable()) {
            decodeFrame()
        }

        val image = imageReader?.acquireLatestImage()
        if (image != null) {
            val hardwareBuffer = image.hardwareBuffer
            image.close() // Make sure to close the Image to free up the buffer
            return hardwareBuffer
        }
        return null
    }

    fun seek(timestamp: Long) {
        // Seek to the closest sync frame at or before the specified time
        extractor.seekTo(timestamp * 1000, MediaExtractor.SEEK_TO_PREVIOUS_SYNC)
        // Flush the codec to reset internal state and buffers
        if (this::decoder::isInitialized.get()) {
            decoder.flush()
        }
    }

    private fun selectVideoTrack(extractor: MediaExtractor): Int {
        val numTracks = extractor.trackCount
        for (i in 0 until numTracks) {
            val format = extractor.getTrackFormat(i)
            val mime = format.getString(MediaFormat.KEY_MIME)
            if (mime?.startsWith("video/") == true) {
                return i
            }
        }
        return -1
    }

    private fun decoderOutputAvailable(): Boolean {
        val info = MediaCodec.BufferInfo()
        val outputBufferId = decoder.dequeueOutputBuffer(info, 0)
        if (outputBufferId >= 0) {
            // If a buffer is available, release it immediately back since we are just checking
            decoder.releaseOutputBuffer(outputBufferId, true)
            return true
        }
        return false
    }

    private fun decodeFrame() {
        val info = MediaCodec.BufferInfo()
        val timeoutUs: Long = 10000
        var isEOS = false

        val inputBufferId = decoder.dequeueInputBuffer(timeoutUs)
        if (inputBufferId >= 0) {
            val inputBuffer = decoder.getInputBuffer(inputBufferId) ?: return
            val sampleSize = extractor.readSampleData(inputBuffer, 0)
            if (sampleSize < 0) {
                // End of stream, make sure to send this information to the decoder
                decoder.queueInputBuffer(
                    inputBufferId, 0, 0, 0L, MediaCodec.BUFFER_FLAG_END_OF_STREAM
                )
                isEOS = true
            } else {
                val presentationTimeUs = extractor.sampleTime
                decoder.queueInputBuffer(inputBufferId, 0, sampleSize, presentationTimeUs, 0)
                extractor.advance()
            }
        }

        val outputBufferId = decoder.dequeueOutputBuffer(info, timeoutUs)
        if (outputBufferId >= 0) {
            // If we have a valid buffer, release it to make it available to the ImageReader's surface
            decoder.releaseOutputBuffer(outputBufferId, true)

            if ((info.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                isEOS = true
            }
        }
    }

    fun release() {
        if (this::decoder::isInitialized.get()) {
            decoder.stop()
            decoder.release()
        }
        if (this::extractor::isInitialized.get()) {
            extractor.release()
        }
    }
}