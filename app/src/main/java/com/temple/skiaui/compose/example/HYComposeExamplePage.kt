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
import com.temple.skiaui.compose.core.Button
import com.temple.skiaui.compose.core.EditText
import com.temple.skiaui.compose.core.ExoVideo
import com.temple.skiaui.compose.core.HYComposeBasePage
import com.temple.skiaui.compose.core.HYComposeSDK
import com.temple.skiaui.compose.core.Icon
import com.temple.skiaui.compose.core.Image
import com.temple.skiaui.compose.core.LazyColumn
import com.temple.skiaui.compose.core.Loading
import com.temple.skiaui.compose.core.Lottie
import com.temple.skiaui.compose.core.Page
import com.temple.skiaui.compose.core.Row
import com.temple.skiaui.compose.core.SVG
import com.temple.skiaui.compose.core.Shader
import com.temple.skiaui.compose.core.Switch
import com.temple.skiaui.compose.core.Text
import com.temple.skiaui.compose.core.View
import com.temple.skiaui.compose.core.Web
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setJustifyContent
import com.temple.skiaui.compose.foundation.setMargins
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setTextSize
import com.temple.skiaui.compose.foundation.setWidth
import com.temple.skiaui.compose.widget.HYComposeView
import kotlin.math.absoluteValue
import kotlin.random.Random

class HYComposeExamplePage(engine: HYSkiaEngine, context: Long) :
    HYComposeBasePage(engine, context) {

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
        Page(modifier = Modifier(context).setSize(width, height)) {
            LazyColumn(
                modifier = Modifier(context).setSize(width, height)
                    .setAlignItems("flex-start"),
                "#ffffff"
            ) {
                View(
                    modifier = Modifier(context)
                        .setSize(200, 200)
                        .onClick { view: HYComposeView ->
                            color = randomColor()
                        },
                    color
                )
                Row(
                    modifier = Modifier(context).setWidth(width)
                        .setAlignItems("center")
                        .setJustifyContent("center")
                ) {
                    SVG(
                        modifier = Modifier(context)
                            .setSize(480, 480)
                            .setBackgroundColor("#ffffff00")
                            .onClick {
                                HYComposeAnimationPage(engine, context).apply {
                                    start(width, height)
                                    HYComposeSDK.pushPage(this)
                                }
                            },
                        source = "jetpack-compose.svg",
                        rotateZ.absoluteValue
                    )
                    Text(
                        modifier = Modifier(context)
                            .setTextSize(40)
                            .setBackgroundColor("#00ffff00"),
                        content = "rememberInfiniteTransition",
                        color = "#ff00ff",
                    )
                }
                Text(
                    modifier = Modifier(context)
                        .setSize(800, 100)
                        .setTextSize(50)
                        .setBackgroundColor("#ffffff00"),
                    content = "ExoPlayer-Video",
                    color = "#0000ff",
                )
                ExoVideo(
                    modifier = Modifier(context).setSize(width, 360 * width / 640),
                    "yiluxiangbei.mp4"
                )
                EditText(
                    modifier = Modifier(context).setSize(width, 150)
                        .setMargins(intArrayOf(0, 50, 0, 0)),
                    "这是Android原生的EditText融合渲染",
                    true
                )
                ComposeCamera(width, height)
                ComposeFilament(width, height)
                Loading(
                    modifier = Modifier(context)
                        .setSize(500, 200)
                        .setMargins(intArrayOf(0, 50, 0, 0))
                        .onClick {
                            color = randomColor()
                        },
                    color = color
                )
                Text(
                    modifier = Modifier(context)
                        .setSize(800, 100)
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
                    modifier = Modifier(context)
                        .setSize(375, 240)
                        .setMargins(intArrayOf(0, 50, 0, 0))
                        .onClick {
                            lottiePlay = !lottiePlay
                        },
                    "WorkspacePlanet.json",
                    lottiePlay
                )
                Shader(
                    modifier = Modifier(context)
                        .setSize(540, 540)
                        .setMargins(intArrayOf(0, 50, 0, 0))
                        .onClick { view: HYComposeView ->
                            if (shaderSource.list.isEmpty()) {
                                shaderSource = ShaderSource("raining.glsl", arrayOf("raining.png"))
                            } else {
                                shaderSource = ShaderSource("sincos.glsl", arrayOf())
                            }
                        },
                    shaderSource
                )
                Image(
                    modifier = Modifier(context)
                        .setSize(300, 300)
                        .setMargins(intArrayOf(0, 50, 0, 0)), source = "bird.gif"
                )
                Row(
                    modifier = Modifier(context).setWidth(width)
                        .setJustifyContent("space-between")
                        .setAlignItems("center")
                        .setMargins(intArrayOf(0, 50, 0, 0))
                ) {
                    Icon(modifier = Modifier(context), 0xe615, color = "#ff0000")
                    Icon(modifier = Modifier(context), 0xe7ce, color = "#ffff00")
                    Icon(modifier = Modifier(context), 0xe670)
                    Icon(modifier = Modifier(context), 0xe67d, color = "#00ff00")
                    Icon(modifier = Modifier(context), 0xe606, color = "#00ffff")
                    Icon(modifier = Modifier(context), 0xe6a2, color = "#000000")
                    Icon(modifier = Modifier(context), 0xe61f)
                }
                Switch(Modifier(context).setMargins(intArrayOf(0, 50, 0, 0)))
                Web(
                    Modifier(context)
                        .setSize(width, 1200)
                        .setMargins(intArrayOf(0, 50, 0, 0)), "https://m.bilibili.com/"
                )
            }
        }
    }

    @Composable
    private fun ComposeCamera(width: Int, height: Int) {
        Button(
            modifier = Modifier(context)
                .setSize(540, 100)
                .setTextSize(60)
                .setMargins(intArrayOf(0, 50, 0, 0))
                .onClick {
                    HYComposeCameraPage(engine, context).apply {
                        start(width, height)
                        HYComposeSDK.pushPage(this)
                    }
                },
            "Camera Page",
        )
    }

    @Composable
    private fun ComposeFilament(width: Int, height: Int) {
        Button(
            modifier = Modifier(context)
                .setSize(540, 100)
                .setTextSize(60)
                .setMargins(intArrayOf(0, 50, 0, 0))
                .onClick {
                    HYComposeFilamentPage(engine, context).apply {
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
