package com.temple.skiaui.compose.example

import android.content.Intent
import android.provider.MediaStore
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.DpSize
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.fillMaxWidth
import com.temple.skiaui.compose.foundation.flex
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.position
import com.temple.skiaui.compose.foundation.wrap
import com.temple.skiaui.compose.runtime.Button
import com.temple.skiaui.compose.runtime.Column
import com.temple.skiaui.compose.runtime.ExoVideo
import com.temple.skiaui.compose.runtime.Loading
import com.temple.skiaui.compose.runtime.ProgressBar
import com.temple.skiaui.compose.runtime.Row
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexWrap
import com.temple.skiaui.compose.ui.Position
import com.temple.skiaui.compose.ui.TextAlign
import com.temple.skiaui.platform.video.ExoPlayerImpl
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class HYComposeVideoListPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            var musicList: Map<String, String>? by remember {
                mutableStateOf(null)
            }
            var videoSrc by remember {
                mutableStateOf("yiluxiangbei.mp4")
            }
            var backgroundPlayback by remember {
                mutableStateOf(false)
            }
            var repeatMode by remember {
                mutableStateOf(false)
            }
            var shaderPath by remember {
                mutableStateOf("")
            }
            var videoWidthHeightRatio by remember {
                mutableFloatStateOf(640.0f / 360.0f)
            }
            var currentPos by remember {
                mutableStateOf("00:000")
            }
            var totalDuration by remember {
                mutableStateOf("00:000")
            }
            var progress by remember {
                mutableIntStateOf(0)
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
                        if (!repeatMode && musicList?.isNotEmpty() == true) {
                            musicList?.entries?.toList()?.random()?.apply {
                                videoSrc = this.value
                            }
                        }
                    }
                }
            }
            LaunchedEffect(Unit) {
                CoroutineScope(Dispatchers.IO).launch {
                    val files = getMusicList()
                    delay(1000)
                    withContext(Dispatchers.Main) {
                        musicList = files
                    }
                }
                while (true) {
                    delay(500)
                    CoroutineScope(Dispatchers.Main).launch {
                        val current = exoplayer.getCurrentPosition()
                        val total = exoplayer.getDuration()
                        if (total <= 0L) {
                            return@launch
                        }
                        currentPos = formatMillisToTime(current)
                        totalDuration = formatMillisToTime(total)
                        progress = (current * 100 / total).toInt()
                    }
                }
            }
            DisposableEffect(Unit) {
                onDispose {
                    exoplayer.release()
                    handleVideoService(false)
                }
            }
            Column(
                modifier = Modifier.size(width, height)
                    .alignItems(Align.Center)
                    .backgroundColor(MaterialTheme.colorScheme.background)
            ) {
                val desiredSize = getDesiredExoSize(width, height, videoWidthHeightRatio)
                ExoVideo(
                    modifier = Modifier.size(desiredSize.width, desiredSize.height),
                    customPlayer = exoplayer,
                    source = videoSrc,
                    shaderPath = shaderPath,
                    backgroundPlayback = backgroundPlayback,
                    repeat = repeatMode
                )
                Row(
                    modifier = Modifier.size(desiredSize.width, desiredSize.height)
                        .backgroundColor(Color.Transparent)
                        .alignItems(Align.FlexEnd)
                        .position(Position.Absolute)
                ) {
                    Text(
                        modifier = Modifier.backgroundColor(Color.Transparent)
                            .margins(arrayOf(0.dp, 0.dp, 0.dp, 0.dp)).flex(1),
                        textSize = 10.dp,
                        color = Color.White,
                        content = currentPos,
                        textAlign = TextAlign.kCenter
                    )
                    ProgressBar(
                        modifier = Modifier.height(12.dp).flex(8),
                        barColor = MaterialTheme.colorScheme.inverseSurface,
                        backgroundColor = MaterialTheme.colorScheme.surfaceVariant,
                        progress = progress,
                        onChange = {
                            exoplayer.seekTo(it * exoplayer.getDuration() / 100)
                        }
                    )
                    Text(
                        modifier = Modifier.backgroundColor(Color.Transparent)
                            .margins(arrayOf(0.dp, 0.dp, 0.dp, 0.dp)).flex(1),
                        textSize = 10.dp,
                        color = Color.White,
                        content = totalDuration,
                        textAlign = TextAlign.kCenter
                    )
                }
                Scroll(
                    modifier = Modifier()
                        .fillMaxWidth(1.0f)
                        .flex(1).alignItems(Align.FlexStart),
                    backgroundColor = MaterialTheme.colorScheme.background
                ) {
                    Text(
                        modifier = Modifier.backgroundColor(Color.Transparent)
                            .margins(arrayOf(0.dp, 10.dp, 0.dp, 0.dp)),
                        textSize = 20.dp,
                        content = stringResource(R.string.local_video_list),
                        color = MaterialTheme.colorScheme.primary,
                    )
                    if (musicList == null) {
                        Loading(
                            modifier = Modifier.size(200.dp, 50.dp)
                                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        )
                    } else if (musicList?.isEmpty() == true) {
                        Text(
                            modifier = Modifier.backgroundColor(Color.Transparent)
                                .margins(arrayOf(0.dp, 10.dp, 0.dp, 0.dp)),
                            textSize = 30.dp,
                            content = stringResource(R.string.empty_video_list),
                            color = MaterialTheme.colorScheme.error,
                        )
                    } else {
                        Row(
                            modifier = Modifier.width(width).backgroundColor(Color.Transparent)
                                .wrap(FlexWrap.Wrap)
                        ) {
                            musicList?.map {
                                ComposeVideoList(it.key, it.value) {
                                    videoSrc = it
                                }
                            }
                        }
                    }
                    Text(
                        modifier = Modifier.backgroundColor(Color.Transparent)
                            .margins(arrayOf(0.dp, 10.dp, 0.dp, 0.dp)),
                        textSize = 20.dp,
                        content = stringResource(R.string.shader_effect),
                        color = MaterialTheme.colorScheme.primary,
                    )
                    Row(
                        modifier = Modifier.width(width).backgroundColor(Color.Transparent)
                            .wrap(FlexWrap.Wrap)
                    ) {
                        mapOf(
                            "black-white shader" to "skia_video_black_white.glsl",
                            "lightning shader" to "skia_video_lightning_shader.glsl",
                            "raining shader" to "skia_video_raining_shader.glsl",
                            "bright shader" to "skia_video_bright_shader.glsl"
                        ).map {
                            ComposeShaderList(it.key, it.value) {
                                shaderPath = it
                            }
                        }
                    }
                    Text(
                        modifier = Modifier.backgroundColor(Color.Transparent)
                            .margins(arrayOf(0.dp, 10.dp, 0.dp, 0.dp)),
                        textSize = 20.dp,
                        content = stringResource(R.string.play_setting),
                        color = MaterialTheme.colorScheme.primary,
                    )
                    Row(
                        modifier = Modifier.width(width).backgroundColor(Color.Transparent)
                            .wrap(FlexWrap.Wrap)
                    ) {
                        Button(
                            modifier = Modifier.margins(arrayOf(0.dp, 10.dp, 10.dp, 0.dp))
                                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
                            content = if (backgroundPlayback) stringResource(R.string.close_background_playback) else stringResource(
                                R.string.open_background_playback
                            ),
                            textSize = 15.dp,
                            color = MaterialTheme.colorScheme.tertiary,
                            onClick = {
                                backgroundPlayback = !backgroundPlayback
                                handleVideoService(backgroundPlayback)
                            })
                        Button(
                            modifier = Modifier.margins(arrayOf(0.dp, 10.dp, 10.dp, 0.dp))
                                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
                            content = if (repeatMode) stringResource(R.string.random_mode) else stringResource(
                                R.string.repeat_mode
                            ),
                            textSize = 15.dp,
                            color = MaterialTheme.colorScheme.tertiary,
                            onClick = {
                                repeatMode = !repeatMode
                            })
                    }
                }
            }
        }
    }

    @Composable
    private fun ComposeVideoList(name: String, path: String, callback: (path: String) -> Unit) {
        Button(
            modifier = Modifier.margins(arrayOf(0.dp, 10.dp, 10.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = name,
            textSize = 15.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                callback(path)
            })
    }

    @Composable
    private fun ComposeShaderList(shader: String, path: String, callback: (path: String) -> Unit) {
        Button(
            modifier = Modifier.margins(arrayOf(0.dp, 10.dp, 10.dp, 0.dp)),
            content = shader,
            textSize = 15.dp,
            color = Color.White,
            onClick = {
                callback(path)
            })
    }

    private fun getMusicList(): Map<String, String> {
        val fileMap = mutableMapOf<String, String>()
        val projection = arrayOf(
            MediaStore.Video.Media._ID,
            MediaStore.Video.Media.DATA,
            MediaStore.Video.Media.DISPLAY_NAME,
        )
        val selection = "${MediaStore.Video.Media.DATA} LIKE ?"
        val selectionArgs = arrayOf("%/skiadata/music/%")
        engine.getContext().contentResolver.query(
            MediaStore.Video.Media.EXTERNAL_CONTENT_URI,
            projection,
            selection,
            selectionArgs,
            "${MediaStore.Video.Media.DATE_MODIFIED} DESC"
        )?.use { cursor ->
            while (cursor.moveToNext()) {
                val name = cursor.getString(
                    cursor.getColumnIndexOrThrow(MediaStore.Video.Media.DISPLAY_NAME)
                )
                val path = cursor.getString(
                    cursor.getColumnIndexOrThrow(MediaStore.Video.Media.DATA)
                )
                fileMap[name] = "file://${path}"
            }
        }
        return fileMap
    }

    private fun formatMillisToTime(millis: Long): String {
        val seconds = millis / 1000
        val minutes = seconds / 60
        return "%02d:%02d".format(minutes % 60, seconds % 60)
    }

    private fun getDesiredExoSize(width: Dp, height: Dp, videoWidthHeightRatio: Float): DpSize {
        if (width < height) {
            return DpSize(width, width / videoWidthHeightRatio)
        } else {
            return DpSize(height * videoWidthHeightRatio, height)
        }
    }

    private fun handleVideoService(backgroundPlayback: Boolean) {
        if (backgroundPlayback) {
            val intent = Intent(engine.getContext(), HYComposeVideoService::class.java)
            engine.getContext().startForegroundService(intent)
        } else {
            val intent = Intent(engine.getContext(), HYComposeVideoService::class.java)
            engine.getContext().stopService(intent)
        }
    }

}
