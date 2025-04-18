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
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.runtime.Button
import com.temple.skiaui.compose.runtime.ExoVideo
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.HYComposeSDK
import com.temple.skiaui.compose.runtime.Icon
import com.temple.skiaui.compose.runtime.Image
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.runtime.Loading
import com.temple.skiaui.compose.runtime.Lottie
import com.temple.skiaui.compose.runtime.Row
import com.temple.skiaui.compose.runtime.SVG
import com.temple.skiaui.compose.runtime.Shader
import com.temple.skiaui.compose.runtime.Switch
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.runtime.View
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.corner
import com.temple.skiaui.compose.foundation.justifyContent
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.textSize
import com.temple.skiaui.compose.material.Badge
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.HYComposeView
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.util.px2dp
import kotlin.math.absoluteValue

class HYComposeExamplePage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
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
        LazyColumn(
            modifier = Modifier.size(width, height)
                .alignItems(Align.FlexStart),
            MaterialTheme.colorScheme.background
        ) {
            View(
                modifier = Modifier.size(if (isSystemInDarkTheme()) 180.dp else 80.dp, 80.dp)
                    .onClick { view: HYComposeView ->
                        color = randomColor()
                    },
                color
            )
            Row(
                modifier = Modifier.width(width)
                    .justifyContent(Justify.SpaceBetween)
                    .alignItems(Align.Center)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                    .backgroundColor(Color.Transparent)
            ) {
                Badge("")
                Badge("1")
                Badge("10")
                Badge("99")
                Badge("100")
                Badge("1000+")
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
                        .onClick {
                            HYComposeMaterialPage(engine).apply {
                                start(width, height)
                                HYComposeSDK.pushPage(this)
                            }
                        },
                    source = "jetpack-compose.svg",
                    rotateZ.absoluteValue
                )
                Text(
                    modifier = Modifier.textSize(16.dp)
                        .backgroundColor(Color.Transparent),
                    content = stringResource(R.string.remember_infinite_transition),
                    color = MaterialTheme.colorScheme.onSurface,
                )
            }
            Text(
                modifier = Modifier
                    .textSize(20.dp)
                    .backgroundColor(Color.Transparent),
                content = stringResource(R.string.exo_player),
                color = MaterialTheme.colorScheme.primary,
            )
            ExoVideo(
                modifier = Modifier.size(width, width.times(360).div(640)).corner(20.dp),
                "yiluxiangbei.mp4"
            )
            ComposeNative(width, height)
            ComposeFilament(width, height)
            Loading(
                modifier = Modifier.size(200.dp, 50.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                    .onClick {
                        color = randomColor()
                    },
                color = color
            )
            Text(
                modifier = Modifier.size(width, 100.dp)
                    .textSize(20.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                    .onClick {
                        ellipsis = !ellipsis
                    },
                content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmojiShow",
                color = MaterialTheme.colorScheme.inversePrimary,
                maxLine = if (ellipsis) 3 else 0,
                ellipsis = stringResource(R.string.click_to_open)
            )
            Lottie(
                modifier = Modifier.size(160.dp, 120.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                    .onClick {
                        lottiePlay = !lottiePlay
                    },
                "WorkspacePlanet.json",
                lottiePlay
            )
            Shader(
                modifier = Modifier
                    .size(200.dp, 200.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                    .onClick { _: HYComposeView ->
                        if (shaderSource.list.isEmpty()) {
                            shaderSource = ShaderSource("raining.glsl", arrayOf("raining.png"))
                        } else {
                            shaderSource = ShaderSource("sincos.glsl", arrayOf())
                        }
                    },
                shaderSource
            )
            Image(
                modifier = Modifier
                    .size(150.dp, 150.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)), source = "bird.gif"
            )
            Row(
                modifier = Modifier.width(width)
                    .justifyContent(Justify.SpaceBetween)
                    .alignItems(Align.Center)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                    .backgroundColor(Color.Transparent)
            ) {
                Icon(0xe615, color = Color.Red)
                Icon(0xe7ce, color = Color.Yellow)
                Icon(0xe670)
                Icon(0xe67d, color = Color.Green)
                Icon(0xe606, color = Color.Cyan)
                Icon(0xe6a2, color = Color.Black)
                Icon(0xe61f)
            }
            Switch(Modifier.margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)))
        }
    }

    @Composable
    private fun ComposeNative(width: Dp, height: Dp) {
        Button(
            modifier = Modifier.textSize(20.dp)
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer)
                .onClick {
                    HYComposeNativeViewsPage(engine).apply {
                        start(width, height)
                        HYComposeSDK.pushPage(this)
                    }
                },
            stringResource(R.string.native_views_page),
            MaterialTheme.colorScheme.tertiary
        )
    }

    @Composable
    private fun ComposeFilament(width: Dp, height: Dp) {
        Button(
            modifier = Modifier.textSize(20.dp)
                .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer)
                .onClick {
                    HYComposeFilamentPage(engine).apply {
                        start(width, height)
                        HYComposeSDK.pushPage(this)
                    }
                },
            stringResource(R.string.filament_page),
            MaterialTheme.colorScheme.tertiary
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
