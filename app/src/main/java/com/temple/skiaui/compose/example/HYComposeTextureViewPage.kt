package com.temple.skiaui.compose.example

import android.util.Log
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.DpSize
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.runtime.TextureView
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.platform.video.ExoPlayerImpl

class HYComposeTextureViewPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            var videoWidthHeightRatio by remember {
                mutableFloatStateOf(1.0f)
            }
            val exoplayer = remember {
                object : ExoPlayerImpl() {
                    override fun onVideoSizeChanged(videoWidth: Int, videoHeight: Int) {
                        super.onVideoSizeChanged(videoWidth, videoHeight)
                        if (videoWidth == 0 || videoHeight == 0) {
                            return
                        }
                        videoWidthHeightRatio = videoWidth * 1.0f / videoHeight
                    }

                    override fun onPlayEnd() {
                        super.onPlayEnd()
                    }
                }
            }
            DisposableEffect(Unit) {
                onDispose {
                    exoplayer.release()
                }
            }
            Scroll(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.Center),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                val desiredSize = getDesiredExoSize(width, height, videoWidthHeightRatio)
                TextureView(
                    modifier = Modifier()
                        .size(desiredSize.width, desiredSize.height),
                    engine = engine,
                    surfaceCreate = { surface ->
                        run {
                            Log.d(TAG, "surfaceCreate")
                            exoplayer.setSource("yiluxiangbei.mp4")
                            exoplayer.setVideoSurface(surface)
                            exoplayer.prepare()
                            exoplayer.play()
                        }
                    },
                    surfaceChange = { surface, width, height ->
                        run {
                            Log.d(TAG, "surfaceChange ${width}: ${height}")
                        }
                    },
                    surfaceDestroy = { surface ->
                        run {
                            Log.d(TAG, "surfaceDestroy")
                            exoplayer.setVideoSurface(null)
                            exoplayer.stop()
                            exoplayer.release()
                        }
                    },
                    show = {
                        run {
                            exoplayer.play()
                        }
                    },
                    hide = {
                        run {
                            exoplayer.pause()
                        }
                    }
                )
            }
        }
    }

    private fun getDesiredExoSize(width: Dp, height: Dp, videoWidthHeightRatio: Float): DpSize {
        if (width < height) {
            return DpSize(width, width / videoWidthHeightRatio)
        } else {
            return DpSize(height * videoWidthHeightRatio, height)
        }
    }

    companion object {
        private const val TAG = "HYComposeTextureViewPage"
    }
}
