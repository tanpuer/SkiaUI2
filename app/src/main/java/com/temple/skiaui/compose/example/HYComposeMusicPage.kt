package com.temple.skiaui.compose.example

import android.util.Log
import androidx.compose.animation.core.LinearEasing
import androidx.compose.animation.core.animateFloat
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.audio.HYSkiaAudioTracker
import com.temple.skiaui.compose.example.lyric.Lyric
import com.temple.skiaui.compose.example.lyric.LyricsViewModel
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.alignSelf
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.corner
import com.temple.skiaui.compose.foundation.justifyContent
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.paddings
import com.temple.skiaui.compose.foundation.position
import com.temple.skiaui.compose.runtime.AndroidImage
import com.temple.skiaui.compose.runtime.Column
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.ProgressBar
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.runtime.Row
import com.temple.skiaui.compose.runtime.SVG
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.ContentScale
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.Position
import com.temple.skiaui.compose.ui.TextGradient
import kotlin.math.absoluteValue


class HYComposeMusicPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            val audioPlayer = remember { HYSkiaAudioTracker("feng.mp4", engine) }
            val viewModel = remember { LyricsViewModel() }
            val lyrics = viewModel.lyrics.collectAsState()
            val progress = viewModel.progress.collectAsState()
            val infiniteTransition = rememberInfiniteTransition()
            val rotateZ by infiniteTransition.animateFloat(
                0.0f,
                360.0f,
                infiniteRepeatable(tween(durationMillis = 10000, easing = LinearEasing))
            )
            remember(rotateZ) {
                viewModel.setProgress(audioPlayer.getCurrentPosition())
                null
            }
            LaunchedEffect(Unit) {
                viewModel.parseLrc("feng.srt")
            }
            DisposableEffect(Unit) {
                onDispose {
                    audioPlayer.release()
                }
            }
            Column(
                modifier = Modifier.size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                AndroidImage(
                    modifier = Modifier.size(width, height)
                        .position(Position.Absolute),
                    source = "music/bg.png",
                    contentScale = ContentScale.Cover,
                    blur = 10f
                )
                Row(
                    modifier = Modifier.width(width)
                        .backgroundColor(Color.Transparent)
                        .paddings(arrayOf(10.dp, 0.dp, 10.dp, 0.dp))
                        .alignItems(Align.Center)
                        .justifyContent(Justify.SpaceAround)
                ) {
                    AndroidImage(
                        modifier = Modifier.size(170.dp / 2, 46.dp / 2)
                            .backgroundColor(Color.Transparent),
                        source = "music/logo.png"
                    )
                    ComposeControl(audioPlayer, viewModel)
                }
                ProgressBar(
                    modifier = Modifier.size(width, 20.dp)
                        .paddings(arrayOf(20.dp, 0.dp, 20.dp, 0.dp)),
                    barColor = Color.Green,
                    backgroundColor = Color.White,
                    progress = (if (audioPlayer.getDuration() == 0L) 0 else progress.value * 100 / audioPlayer.getDuration()).toInt(),
                    onChange = {
                        audioPlayer.seek(audioPlayer.getDuration() * it / 100)
                    }
                )
                SVG(
                    modifier = Modifier.size(40.dp, 40.dp).backgroundColor(Color.Transparent)
                        .margins(arrayOf(20.dp, 0.dp, 0.dp, 0.dp)),
                    source = "music/mv_definition_super.svg",
                )
                AndroidImage(
                    modifier = Modifier.size(width, width)
                        .position(Position.Absolute)
                        .backgroundColor(Color.Transparent)
                        .alignSelf(Align.Center)
                        .margins(arrayOf(0.dp, (height - width) / 2, 0.dp, 0.dp)),
                    source = "music/record_player_verydark_highlight.png"
                )
                AndroidImage(
                    modifier = Modifier.size(160.dp, 160.dp)
                        .position(Position.Absolute)
                        .corner(80.dp)
                        .backgroundColor(Color.Transparent)
                        .alignSelf(Align.Center)
                        .margins(arrayOf((width - 160.dp) / 2, (height - 160.dp) / 2, 0.dp, 0.dp)),
                    source = "music/bg.png",
                    rotateZ = rotateZ.absoluteValue
                )
                AndroidImage(
                    modifier = Modifier.size(13.dp * 5, 60.dp * 5)
                        .position(Position.Absolute)
                        .corner(80.dp)
                        .backgroundColor(Color.Transparent)
                        .alignSelf(Align.FlexStart)
                        .margins(arrayOf(width - 120.dp, 150.dp, 50.dp, 0.dp)),
                    source = "music/record_player_dark_arm.png",
                    contentScale = ContentScale.Cover
                )
                LyricsScreen(width, height - 150.dp, progress.value, lyrics.value)
            }
        }
    }

    @Composable
    private fun ComposeControl(audioPlayer: HYSkiaAudioTracker, viewModel: LyricsViewModel) {
        val playing = viewModel.playing.collectAsState()
        Row(
            modifier = Modifier.backgroundColor(Color.Transparent)
                .alignSelf(Align.FlexEnd)
        ) {
            AndroidImage(
                modifier = Modifier.size(40.dp, 40.dp)
                    .backgroundColor(Color.Transparent),
                source = "music/ic_previous.png",
                onClick = {
                    audioPlayer.seek(0)
                    viewModel.setPlaying(true)
                }
            )
            AndroidImage(
                modifier = Modifier.size(40.dp, 40.dp)
                    .backgroundColor(Color.Transparent)
                    .margins(arrayOf(10.dp, 0.dp, 0.dp, 0.dp)),
                source = if (playing.value) "music/ic_pause.png" else "music/ic_play.png",
                onClick = {
                    if (playing.value) {
                        viewModel.setPlaying(false)
                        audioPlayer.pause()
                    } else {
                        viewModel.setPlaying(true)
                        audioPlayer.start()
                    }
                }
            )
            AndroidImage(
                modifier = Modifier.size(40.dp, 40.dp)
                    .backgroundColor(Color.Transparent)
                    .margins(arrayOf(10.dp, 0.dp, 0.dp, 0.dp)),
                source = "music/ic_next.png",
                onClick = {
                    audioPlayer.seek(0)
                    viewModel.setPlaying(true)
                }
            )
        }
    }

    @Composable
    fun LyricsScreen(
        width: Dp,
        height: Dp,
        progress: Long,
        lyrics: List<Lyric>
    ) {
        Log.d(TAG, "progress is $progress, lyrics is ${lyrics.size}")
        val colors = remember { arrayOf(Color.Green, Color.Green, Color.White, Color.White) }
        val index = remember { 0 }
        val currentIndex = calculateIndex(lyrics, progress)
        Scroll(
            modifier = Modifier.size(width, height)
                .backgroundColor(Color.Transparent)
                .alignItems(Align.Center)
        ) {
            lyrics.forEachIndexed { index, lyric ->
                val gradientStops = remember(progress) {
                    if (progress < lyric.timeMills.first() || progress > lyric.timeMills.last()) {
                        floatArrayOf()
                    } else {
                        val percent = calculatePercent(lyric, progress)
                        floatArrayOf(0f, percent, percent, 1f)
                    }
                }
                Text(
                    modifier = Modifier.backgroundColor(Color.Transparent)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 20.dp)),
                    content = lyric.content,
                    textSize = 25.dp,
                    color = Color.White,
                    textGradient = TextGradient(
                        if (gradientStops.size == 4) colors else arrayOf(),
                        gradientStops
                    )
                )
            }
        }
    }

    private fun calculateIndex(lyrics: List<Lyric>, progress: Long): Int {
        var index = -1
        for (i in lyrics.indices) {
            val item = lyrics[i]
            val start = item.timeMills.first()
            val end = item.timeMills.last()
            if (progress in start..end) {
                index = i
                break
            } else if (start > progress) {
                index = i - 1
                break
            }
        }
        return index
    }

    private fun calculatePercent(lyric: Lyric, progress: Long): Float {
        var jIndex = 0
        for (j in 0..<lyric.timeMills.size) {
            if (lyric.timeMills[j] >= progress) {
                jIndex = j
                break
            }
        }
        val totalLength = lyric.content.length
        var gradientLength = 0.0f
        for (z in 0..<jIndex - 1) {
            gradientLength += lyric.contentList[z].length
        }
        if (jIndex >= 1) {
            gradientLength += (progress - lyric.timeMills[jIndex - 1]) * 1.0f /
                    (lyric.timeMills[jIndex] - lyric.timeMills[jIndex - 1]) *
                    lyric.contentList[jIndex - 1].length
        }
        return gradientLength * 1.0f / totalLength
    }

    companion object {
        private const val TAG = "HYComposeMusicPage"
    }

}
