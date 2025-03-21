package com.temple.skiaui.compose.example

import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.Camera
import com.temple.skiaui.compose.core.ExoVideo
import com.temple.skiaui.compose.core.Filament
import com.temple.skiaui.compose.core.Icon
import com.temple.skiaui.compose.core.Image
import com.temple.skiaui.compose.core.HYComposeBasePage
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
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setTextSize
import com.temple.skiaui.compose.foundation.setWidth
import com.temple.skiaui.compose.widget.CameraCallback
import com.temple.skiaui.compose.widget.HYComposeView
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
        Page(modifier = Modifier(context).setSize(width, height)) {
            LazyColumn(
                modifier = Modifier(context).setSize(width, height)
                    .setAlignItems("flex-start"),
                "#00000066"
            ) {
                View(
                    modifier = Modifier(context)
                        .setSize(200, 200)
                        .onClick { view: HYComposeView ->
                            color = randomColor()
                        },
                    color
                )
                SVG(
                    modifier = Modifier(context)
                        .setSize(480, 480)
                        .setBackgroundColor("#ffffff00"),
                    source = "jetpack-compose.svg"
                )
                Text(
                    modifier = Modifier(context)
                        .setSize(800, 100)
                        .setTextSize(50)
                        .setBackgroundColor("#ffffff00"),
                    content = "这是ExoPlayer-Video",
                    color = "#0000ff",
                )
                ExoVideo(
                    modifier = Modifier(context).setSize(width, 360 * width / 640),
                    "yiluxiangbei.mp4"
                )
                Filament(modifier = Modifier(context).setSize(width, 500))
                Camera(Modifier(context).setSize(width, width), object : CameraCallback {
                    override fun onImageCaptured(imagePtr: Long) {

                    }
                })
                Loading(
                    modifier = Modifier(context)
                        .setSize(500, 200)
                        .onClick {
                            color = randomColor()
                        },
                    color = color
                )
                Text(
                    modifier = Modifier(context)
                        .setSize(800, 100)
                        .setTextSize(50)
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
                        .onClick {
                            lottiePlay = !lottiePlay
                        },
                    "WorkspacePlanet.json",
                    lottiePlay
                )
                Shader(
                    modifier = Modifier(context).setSize(540, 540)
                        .onClick { view: HYComposeView ->
                            if (shaderSource.list.isEmpty()) {
                                shaderSource = ShaderSource("raining.glsl", arrayOf("raining.png"))
                            } else {
                                shaderSource = ShaderSource("sincos.glsl", arrayOf())
                            }
                        },
                    shaderSource
                )
                Image(modifier = Modifier(context).setSize(300, 300), source = "bird.gif")
                Row(
                    modifier = Modifier(context).setWidth(width)
                        .setJustifyContent("space-between")
                        .setAlignItems("center")
                ) {
                    Icon(modifier = Modifier(context), 0xe615, color = "#ff0000")
                    Icon(modifier = Modifier(context), 0xe7ce, color = "#ffff00")
                    Icon(modifier = Modifier(context), 0xe670)
                    Icon(modifier = Modifier(context), 0xe67d, color = "#00ff00")
                    Icon(modifier = Modifier(context), 0xe606, color = "#00ffff")
                    Icon(modifier = Modifier(context), 0xe6a2, color = "#000000")
                    Icon(modifier = Modifier(context), 0xe61f)
                }
                Switch(Modifier(context))
                Web(Modifier(context).setSize(width, 1200), "https://m.bilibili.com/")
            }
        }
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
