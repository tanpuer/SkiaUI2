package com.temple.skiaui.compose.example

import android.provider.MediaStore
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.State
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.foundation.wrap
import com.temple.skiaui.compose.runtime.Button
import com.temple.skiaui.compose.runtime.ExoVideo
import com.temple.skiaui.compose.runtime.Loading
import com.temple.skiaui.compose.runtime.Row
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexWrap
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
            var shaderPath by remember {
                mutableStateOf("")
            }
            var videoViewHeight by remember {
                mutableStateOf(200.dp)
            }
            val exoplayer = remember {
                object : ExoPlayerImpl() {
                    override fun onVideoSizeChanged(videoWidth: Int, videoHeight: Int) {
                        super.onVideoSizeChanged(videoWidth, videoHeight)
                        videoViewHeight = width * videoHeight / videoWidth
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
            }
            Scroll(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                ExoVideo(
                    modifier = Modifier.size(width, videoViewHeight),
                    source = videoSrc,
                    shaderPath = shaderPath,
                    customPlayer = exoplayer
                )
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
                        modifier = Modifier.width(width)
                            .backgroundColor(Color.Transparent)
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
                    modifier = Modifier.width(width)
                        .backgroundColor(Color.Transparent)
                        .wrap(FlexWrap.Wrap)
                ) {
                    mapOf(
                        "black-white shader" to "skia_video_black_white.glsl",
                        "lightning shader" to "skia_video_lightning_shader.glsl",
                        "raining shader" to "skia_video_raining_shader.glsl"
                    ).map {
                        ComposeShaderList(it.key, it.value) {
                            shaderPath = it
                        }
                    }
                }

            }
        }
    }

    @Composable
    private fun ComposeVideoList(name: String, path: String, callback: (path: String) -> Unit) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 10.dp, 10.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = name,
            textSize = 15.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                callback(path)
            }
        )
    }

    @Composable
    private fun ComposeShaderList(shader: String, path: String, callback: (path: String) -> Unit) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 10.dp, 10.dp, 0.dp)),
            content = shader,
            textSize = 15.dp,
            color = Color.White,
            onClick = {
                callback(path)
            }
        )
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
}