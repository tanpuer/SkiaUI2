package com.temple.skiaui.compose.example

import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.animateFloat
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
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
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.corner
import com.temple.skiaui.compose.foundation.fillMaxWidth
import com.temple.skiaui.compose.foundation.justifyContent
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.paddings
import com.temple.skiaui.compose.material.Badge
import com.temple.skiaui.compose.runtime.AndroidImage
import com.temple.skiaui.compose.runtime.Button
import com.temple.skiaui.compose.runtime.ExoVideo
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.HYComposeSDK
import com.temple.skiaui.compose.runtime.Icon
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.runtime.Loading
import com.temple.skiaui.compose.runtime.Lottie
import com.temple.skiaui.compose.runtime.ProgressBar
import com.temple.skiaui.compose.runtime.Row
import com.temple.skiaui.compose.runtime.SVG
import com.temple.skiaui.compose.runtime.Shader
import com.temple.skiaui.compose.runtime.Switch
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.runtime.View
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.ContentScale
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.util.px2dp
import kotlin.math.absoluteValue

class HYComposeExamplePage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            var color by remember { mutableStateOf(randomColor()) }
            var shaderSource by remember {
                mutableStateOf(ShaderSource("raining.glsl", arrayOf("raining.png")))
            }
            var ellipsis by remember {
                mutableStateOf(true)
            }
            var lottiePlay by remember {
                mutableStateOf(true)
            }
            val infiniteTransition = rememberInfiniteTransition()
            val rotateZ by infiniteTransition.animateFloat(
                0.0f,
                360.0f,
                infiniteRepeatable(tween(5000), RepeatMode.Reverse)
            )
            var birdBlur by remember {
                mutableFloatStateOf(10.0f)
            }
            Scroll(
                modifier = Modifier.size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                View(
                    modifier = Modifier.size(if (isSystemInDarkTheme()) 180.dp else 80.dp, 80.dp),
                    backgroundColor = color,
                    onClick = {
                        color = randomColor()
                    }
                )
                Row(
                    modifier = Modifier.justifyContent(Justify.SpaceBetween)
                        .alignItems(Align.Center)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(Color.Transparent)
                        .fillMaxWidth(0.7f)
                ) {
                    Badge(content = "")
                    Badge(content = "1")
                    Badge(content = "10")
                    Badge(content = "99")
                    Badge(content = "100")
                    Badge(content = "1000+")
                }
                Row(
                    modifier = Modifier.width(width)
                        .alignItems(Align.Center)
                        .justifyContent(Justify.Center)
                        .backgroundColor(Color.Transparent)
                ) {
                    SVG(
                        modifier = Modifier.size(px2dp(480), px2dp(480))
                            .backgroundColor(Color.Transparent)
                            .margins(arrayOf(1.dp, 1.dp, 0.dp, 0.dp)),
                        source = "jetpack-compose.svg",
                        rotateZ = rotateZ.absoluteValue,
                        onClick = {
                            HYComposeMaterialPage(engine).apply {
                                start(width, height)
                                HYComposeSDK.pushPage(this)
                            }
                        }
                    )
                    Text(
                        modifier = Modifier.backgroundColor(Color.Transparent),
                        textSize = 16.dp,
                        content = stringResource(R.string.remember_infinite_transition),
                        color = MaterialTheme.colorScheme.onSurface,
                    )
                }
                Text(
                    modifier = Modifier.backgroundColor(Color.Transparent),
                    textSize = 20.dp,
                    content = stringResource(R.string.exo_player),
                    color = MaterialTheme.colorScheme.primary,
                    fontFamily = "Aria"
                )
                ExoVideo(
                    modifier = Modifier.size(width, width.times(360).div(640)).corner(20.dp),
                    source = "yiluxiangbei.mp4",
                    repeat = true
                )
                ComposeNative(width, height)
                ComposeCamera(width, height)
                ComposeFilament(width, height)
                ComposeRecycler(width, height)
                ComposeCanvas(width, height)
                ComposeMusic(width, height)
                ComposeVideoList(width, height)
                ComposeTextureView(width, height)
                Loading(
                    modifier = Modifier.size(200.dp, 50.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)),
                    color = color,
                    onClick = {
                        color = randomColor()
                    }
                )
                Text(
                    modifier = Modifier.size(width, 100.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(Color.Transparent),
                    content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmojiShow",
                    textSize = 20.dp,
                    color = MaterialTheme.colorScheme.inversePrimary,
                    maxLine = if (ellipsis) 3 else 0,
                    ellipsis = stringResource(R.string.click_to_open),
                    onClick = {
                        ellipsis = !ellipsis
                    }
                )
                Lottie(
                    modifier = Modifier.size(160.dp, 120.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(Color.Transparent),
                    source = "WorkspacePlanet.json",
                    play = lottiePlay,
                    onClick = {
                        lottiePlay = !lottiePlay
                    }
                )
                Shader(
                    modifier = Modifier
                        .size(200.dp, 200.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)),
                    shaderSource = shaderSource,
                    onClick = {
                        if (shaderSource.list.isEmpty()) {
                            shaderSource = ShaderSource("raining.glsl", arrayOf("raining.png"))
                        } else {
                            shaderSource = ShaderSource("sincos.glsl", arrayOf())
                        }
                    }
                )
                AndroidImage(
                    modifier = Modifier
                        .size(150.dp, 150.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(Color.Transparent),
                    source = "bird.gif",
                    contentScale = ContentScale.Cover,
                    blur = birdBlur
                )
                ProgressBar(
                    modifier = Modifier.size(width, 20.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .paddings(arrayOf(20.dp, 0.dp, 20.dp, 0.dp)),
                    barColor = MaterialTheme.colorScheme.tertiary,
                    backgroundColor = MaterialTheme.colorScheme.tertiaryContainer,
                    progress = 50,
                    onChange = { progress, finished ->
                        birdBlur = progress / 5.0f
                    }
                )
                AndroidImage(
                    modifier = Modifier
                        .size(150.dp, 150.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(Color.Transparent),
                    resId = R.drawable.ic_launcher_background,
                    contentScale = ContentScale.Cover
                )
                Row(
                    modifier = Modifier.width(width)
                        .justifyContent(Justify.SpaceBetween)
                        .alignItems(Align.Center)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(Color.Transparent)
                ) {
                    Icon(icon = 0xe615, color = Color.Red)
                    Icon(icon = 0xe7ce, color = Color.Yellow)
                    Icon(icon = 0xe670)
                    Icon(icon = 0xe67d, color = Color.Green)
                    Icon(icon = 0xe606, color = Color.Cyan)
                    Icon(icon = 0xe6a2, color = Color.Black)
                    Icon(icon = 0xe61f)
                }
                Switch(Modifier.margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)))
            }
        }
    }

    @Composable
    private fun ComposeNative(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.native_views_page),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeNativeViewsPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeFilament(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.filament_page),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeFilamentPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeCamera(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.camera_page),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeCameraPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeRecycler(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.recycler_view_page),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeRecyclerPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeCanvas(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.canvas_page),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeCanvasPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeMusic(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.qq_music_page),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeMusicPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeVideoList(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.local_video_list),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeVideoListPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    @Composable
    private fun ComposeTextureView(width: Dp, height: Dp) {
        Button(
            modifier = Modifier
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
            content = stringResource(R.string.skia_texture_view),
            textSize = 20.dp,
            color = MaterialTheme.colorScheme.tertiary,
            onClick = {
                HYComposeTextureViewPage(engine).apply {
                    start(width, height)
                    HYComposeSDK.pushPage(this)
                }
            }
        )
    }

    private fun randomColor(): Color {
        return Color(
            (Math.random() * 255).toInt(),
            (Math.random() * 255).toInt(),
            (Math.random() * 255).toInt(),
            255,
        )
    }

}
