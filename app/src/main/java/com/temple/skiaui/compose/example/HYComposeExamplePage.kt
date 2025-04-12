package com.temple.skiaui.compose.example

import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.animateFloat
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import com.temple.skiaui.HYSkiaEngine
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
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setJustifyContent
import com.temple.skiaui.compose.foundation.setMargins
import com.temple.skiaui.compose.foundation.setTextSize
import com.temple.skiaui.compose.ui.HYComposeView
import kotlin.math.absoluteValue
import kotlin.random.Random

class HYComposeExamplePage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
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
            modifier = Modifier.setSize(width, height)
                .setAlignItems("flex-start"),
            "#00000066"
        ) {
            View(
                modifier = Modifier.setSize(200, 200)
                    .onClick { view: HYComposeView ->
                        color = randomColor()
                    },
                color
            )
            Row(
                modifier = Modifier.setWidth(width)
                    .setAlignItems("center")
                    .setJustifyContent("center")
                    .setBackgroundColor("#ffffff00")
            ) {
                SVG(
                    modifier = Modifier.setSize(480, 480)
                        .setBackgroundColor("#ffffff00")
                        .onClick {
                            HYComposeAnimationPage(engine).apply {
                                start(width, height)
                                HYComposeSDK.pushPage(this)
                            }
                        },
                    source = "jetpack-compose.svg",
                    rotateZ.absoluteValue
                )
                Text(
                    modifier = Modifier.setTextSize(40)
                        .setBackgroundColor("#00ffff00"),
                    content = "rememberInfiniteTransition",
                    color = "#ff00ff",
                )
            }
            Text(
                modifier = Modifier
                    .setSize(800, 100)
                    .setTextSize(50)
                    .setBackgroundColor("#ffffff00"),
                content = "ExoPlayer-Video",
                color = "#0000ff",
            )
            ExoVideo(
                modifier = Modifier.setSize(width, 360 * width / 640),
                "yiluxiangbei.mp4"
            )
            ComposeNative(width, height)
            ComposeFilament(width, height)
            Loading(
                modifier = Modifier.setSize(500, 200)
                    .setMargins(intArrayOf(0, 50, 0, 0))
                    .onClick {
                        color = randomColor()
                    },
                color = color
            )
            Text(
                modifier = Modifier.setSize(800, 100)
                    .setTextSize(50)
                    .setMargins(intArrayOf(0, 50, 0, 0))
                    .onClick {
                        ellipsis = !ellipsis
                    },
                content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmojiShow",
                color = "#0000ff",
                maxLine = if (ellipsis) 3 else 0,
                ellipsis = "点击展开"
            )
            Lottie(
                modifier = Modifier.setSize(375, 240)
                    .setMargins(intArrayOf(0, 50, 0, 0))
                    .onClick {
                        lottiePlay = !lottiePlay
                    },
                "WorkspacePlanet.json",
                lottiePlay
            )
            Shader(
                modifier = Modifier
                    .setSize(540, 540)
                    .setMargins(intArrayOf(0, 50, 0, 0))
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
                    .setSize(300, 300)
                    .setMargins(intArrayOf(0, 50, 0, 0)), source = "bird.gif"
            )
            Row(
                modifier = Modifier.setWidth(width)
                    .setJustifyContent("space-between")
                    .setAlignItems("center")
                    .setMargins(intArrayOf(0, 50, 0, 0))
                    .setBackgroundColor("#ffffff00")
            ) {
                Icon(0xe615, color = "#ff0000")
                Icon(0xe7ce, color = "#ffff00")
                Icon(0xe670)
                Icon(0xe67d, color = "#00ff00")
                Icon(0xe606, color = "#00ffff")
                Icon(0xe6a2, color = "#000000")
                Icon(0xe61f)
            }
            Switch(Modifier.setMargins(intArrayOf(0, 50, 0, 0)))
        }
    }

    @Composable
    private fun ComposeNative(width: Int, height: Int) {
        Button(
            modifier = Modifier.setTextSize(60)
                .setMargins(intArrayOf(0, 50, 0, 0))
                .onClick {
                    HYComposeNativeViewsPage(engine).apply {
                        start(width, height)
                        HYComposeSDK.pushPage(this)
                    }
                },
            "Native Views Page",
        )
    }

    @Composable
    private fun ComposeFilament(width: Int, height: Int) {
        Button(
            modifier = Modifier.setTextSize(60)
                .setMargins(intArrayOf(0, 50, 0, 0))
                .onClick {
                    HYComposeFilamentPage(engine).apply {
                        start(width, height)
                        HYComposeSDK.pushPage(this)
                    }
                },
            "Filament Page",
        )
    }

    private fun randomColor(): String {
        val hexChars = "0123456789abcdef"
        val nextColor = StringBuilder("#")
        for (i in 0 until 6) {
            nextColor.append(hexChars[Random.nextInt(hexChars.length)])
        }
        return nextColor.toString()
    }

}
