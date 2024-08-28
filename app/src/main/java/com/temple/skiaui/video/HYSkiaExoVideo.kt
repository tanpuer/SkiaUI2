package com.temple.skiaui.video

import android.graphics.ImageFormat
import android.hardware.HardwareBuffer
import android.media.ImageReader
import android.media.ImageReader.OnImageAvailableListener
import android.media.MediaExtractor
import android.media.MediaFormat
import android.net.Uri
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import android.view.Surface
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

class HYSkiaExoVideo internal constructor(
    private val assetsPath: String,
    private val engine: HYSkiaEngine
) {

    private lateinit var extractor: MediaExtractor
    private var imageReader: ImageReader? = null
    private var outputSurface: Surface? = null
    private var duration: Double = 0.0
    private var frameRate: Double = 25.0
    private val decodeThread = HandlerThread("exoplayer-${INDEX++}").apply {
        start()
    }
    private val decodeHandler = Handler(decodeThread.looper)
    private var skImagePtr = 0L

    private var exoPlayer: ExoPlayer? = null

    init {
        decodeHandler.post {
            this.initializeReader()
        }
    }

    fun getCurrentSkImage(): Long {
        return skImagePtr
    }

    @OptIn(UnstableApi::class)
    private fun initializeReader() {
        extractor = MediaExtractor()
        val afd = HYSkiaUIApp.getInstance().assets.openFd(this.assetsPath)
        extractor.setDataSource(afd)
        val trackIndex = selectVideoTrack(extractor)
        if (trackIndex < 0) {
            throw RuntimeException("No video track found in " + this.assetsPath)
        }
        extractor.selectTrack(trackIndex)
        val format = extractor.getTrackFormat(trackIndex)
        if (format.containsKey(MediaFormat.KEY_DURATION)) {
            duration = (format.getLong(MediaFormat.KEY_DURATION) / 1000).toDouble()
        }
        if (format.containsKey(MediaFormat.KEY_FRAME_RATE)) {
            frameRate = format.getInteger(MediaFormat.KEY_FRAME_RATE).toDouble()
        }
        val width = format.getInteger(MediaFormat.KEY_WIDTH)
        val height = format.getInteger(MediaFormat.KEY_HEIGHT)
        extractor.release()
        afd.close()

        exoPlayer = ExoPlayer.Builder(HYSkiaUIApp.getInstance()).build()
        val dataSourceFactory = DataSource.Factory { AssetDataSource(HYSkiaUIApp.getInstance()) }
        val uri = Uri.parse("asset:///$assetsPath")
        val mediaSource: MediaSource = ProgressiveMediaSource.Factory(dataSourceFactory)
            .createMediaSource(MediaItem.fromUri(uri))
        exoPlayer?.setMediaSource(mediaSource)
        imageReader = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            ImageReader.newInstance(
                width, height, ImageFormat.PRIVATE, 2, HardwareBuffer.USAGE_GPU_SAMPLED_IMAGE
            )
        } else {
            ImageReader.newInstance(
                width, height, ImageFormat.PRIVATE, 2
            )
        }
        imageReader?.setOnImageAvailableListener(object : OnImageAvailableListener {
            override fun onImageAvailable(reader: ImageReader?) {
                val image = reader?.acquireLatestImage()
                Log.d(TAG, "${(image?.timestamp ?: 0)/ 1000000}")
                val hardwareBuffer = image?.hardwareBuffer ?: return
                image.close()
                engine.makeHardwareBufferToSkImage(hardwareBuffer) {
                    skImagePtr = it
                }

            }
        }, decodeHandler)
        outputSurface = imageReader?.surface
        exoPlayer?.setVideoSurface(outputSurface)
        exoPlayer?.repeatMode = Player.REPEAT_MODE_ALL
        exoPlayer?.prepare()
        exoPlayer?.play()
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

    fun release() {
        decodeHandler.post {
            exoPlayer?.stop()
            exoPlayer?.release()
            extractor.release()
        }
        decodeThread.quitSafely()
    }

    fun start() {
        decodeHandler.post {
            exoPlayer?.play()
        }
    }

    fun pause() {
        decodeHandler.post {
            exoPlayer?.pause()
        }
    }

    companion object {
        private var INDEX = 1
        private const val TAG = "HYSkiaExoVideo"
    }
}