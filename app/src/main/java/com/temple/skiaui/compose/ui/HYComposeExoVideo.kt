package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.platform.video.IVideoPlayer

class HYComposeExoVideo(modifier: Modifier) : HYComposeBaseSurfaceTexture(modifier) {

    fun setSource(source: String) {
        nativeSetSource(ref, source)
    }

    fun setCustomPlayer(player: IVideoPlayer?) {
        player?.let {
            nativeSetCustomPlayer(ref, it)
        }
    }

    fun setBackgroundPlayback(enable: Boolean?) {
        enable?.let {
            nativeSetBackgroundPlayback(ref, enable)
        }
    }

    fun setRepeatMode(repeatable: Boolean?) {
        repeatable?.let {
            nativeSetRepeatMode(ref, repeatable)
        }
    }

    override fun getViewType(): String = "ExoVideo"

    private external fun nativeSetSource(videoView: Long, source: String)
    private external fun nativeSetCustomPlayer(videoView: Long, player: IVideoPlayer)
    private external fun nativeSetBackgroundPlayback(videoView: Long, enable: Boolean)
    private external fun nativeSetRepeatMode(videoView: Long, enable: Boolean)

}