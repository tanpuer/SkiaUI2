package com.temple.skiaui.video

import android.graphics.ImageFormat
import android.hardware.HardwareBuffer
import android.media.ImageReader
import android.media.MediaCodec
import android.media.MediaExtractor
import android.media.MediaFormat
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.util.Log
import android.view.Surface
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.audio.HYSkiaAudioTracker
import com.temple.skiaui.audio.IAudioCallback
import java.io.IOException

class HYSkiaVideo internal constructor(
    private val assetsPath: String,
    private val engine: HYSkiaEngine
) {

    private lateinit var extractor: MediaExtractor
    private lateinit var decoder: MediaCodec
    private var imageReader: ImageReader? = null
    private var outputSurface: Surface? = null

    private var duration: Double = 0.0

    private var frameRate: Double = 25.0

    private val threadName = "video-decoder${INDEX++}"

    private val decodeThread = HandlerThread(threadName).apply {
        start()
    }

    private val decodeHandler = Handler(decodeThread.looper)

    private val mainHandler = engine.mainHandler

    private var skImagePtr = 0L

    @Volatile
    private var released = false

    private var renderFlag = true

    private var hardwareBuffer: HardwareBuffer? = null

    @Volatile
    private var playing = true

    private var audioTracker: HYSkiaAudioTracker? = null

    private var currentVideoPts: Long = 0L

    private val decodeOneFrameRunnable = object : Runnable {
        override fun run() {
            if (released || !renderFlag) {
                return
            }
            Log.d(TAG, "Video: ${currentVideoPts}, audio:${audioTracker?.getCurrentPosition()}")
            if (currentVideoPts >= (audioTracker?.getCurrentPosition() ?: 0)) {
                decodeHandler.postDelayed(this, (1000 / frameRate).toLong() / 2)
                return
            }
            decodeHandler.postDelayed(this, (1000 / frameRate).toLong() / 2)
            val hardwareBuffer = nextImage() ?: return
            this@HYSkiaVideo.hardwareBuffer = hardwareBuffer
            engine.makeHardwareBufferToSkImage(hardwareBuffer) {
                skImagePtr = it
            }
        }
    }

    private val createListener = fun(it: Boolean) {
        if (it && playing) {
            decodeHandler.post(decodeOneFrameRunnable)
        } else {
            decodeHandler.removeCallbacks(decodeOneFrameRunnable)
            decodeHandler.post {
                skImagePtr = 0L
                hardwareBuffer?.close()
                hardwareBuffer = null
            }
        }
    }

    init {
        decodeHandler.post {
            this.initializeReader()
        }
        decodeHandler.postDelayed(decodeOneFrameRunnable, 100)
        mainHandler.post {
            engine.addSkiaSurfaceListener(threadName, createListener)
        }
        playAudio()
    }

    fun getCurrentSkImage(): Long {
        return skImagePtr
    }

    private fun initializeReader() {
        extractor = MediaExtractor()
        try {
            val afd = HYSkiaUIApp.getInstance().assets.openFd(this.assetsPath)
            extractor.setDataSource(afd)
            val trackIndex = selectVideoTrack(extractor)
            if (trackIndex < 0) {
                throw RuntimeException("No video track found in " + this.assetsPath)
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

    private fun nextImage(): HardwareBuffer? {
        if (!decoderOutputAvailable()) {
            decodeFrame()
        }

        val image = imageReader?.acquireLatestImage()
        if (image != null) {
            if (hardwareBuffer != null) {
                image.close()
                return null
            }
            val hardwareBuffer = image.hardwareBuffer
            image.close()
            return hardwareBuffer
        }
        return nextImage()
    }

    private fun seek(timestamp: Long) {
        currentVideoPts = 0
        extractor.seekTo(timestamp * 1000, MediaExtractor.SEEK_TO_PREVIOUS_SYNC)
        decoder.flush()
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
        val timeoutUs: Long = 2000
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
                seek(0)
            } else {
                val presentationTimeUs = extractor.sampleTime
                currentVideoPts = presentationTimeUs / 1000
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
        released = true
        decodeHandler.post {
            decoder.stop()
            decoder.release()
            extractor.release()
        }
        decodeThread.quitSafely()
        audioTracker?.release()
        mainHandler.post {
            engine.removeSurfaceListener(threadName)
        }
    }

    fun start() {
        playing = true
        decodeHandler.removeCallbacks(decodeOneFrameRunnable)
        decodeHandler.post(decodeOneFrameRunnable)
        audioTracker?.start()
    }

    fun pause() {
        playing = false
        decodeHandler.removeCallbacks(decodeOneFrameRunnable)
        audioTracker?.pause()
    }

    fun deleteSkImage(ptr: Long) {
        engine.deleteSkImage(ptr)
    }

    private fun playAudio() {
        audioTracker = HYSkiaAudioTracker(assetsPath, engine)
        audioTracker?.start()
        audioTracker?.audioCallback = object : IAudioCallback {
            override fun onComplete() {
                decodeHandler.post {
                    seek(0)
                }
            }
        }
    }

    companion object {
        private var INDEX = 1
        private const val TAG = "HYSkiaVideo"
    }
}