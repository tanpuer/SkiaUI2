package com.temple.skiaui.platform.video

import android.view.Surface

interface IVideoPlayer : IVideoListener {

    fun setSource(source: String)

    fun setVideoSurface(surface: Surface?)

    fun setRepeat(flag: Boolean)

    fun prepare()

    fun play()

    fun pause()

    fun stop()

    fun release()

    fun seekTo(positionMs: Long)

    fun getCurrentPosition(): Long

    fun getDuration(): Long

    fun setVideoListener(listener: IVideoListener?)

}

interface IVideoListener {
    fun onRenderedFirstFrame()
    fun onVideoSizeChanged(videoWidth: Int, videoHeight: Int)
}
