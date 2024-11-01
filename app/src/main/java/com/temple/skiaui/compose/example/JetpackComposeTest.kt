package com.temple.skiaui.compose.example

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.Column
import com.temple.skiaui.compose.core.Icon
import com.temple.skiaui.compose.core.Image
import com.temple.skiaui.compose.core.LazyColumn
import com.temple.skiaui.compose.core.Loading
import com.temple.skiaui.compose.core.Lottie
import com.temple.skiaui.compose.core.Row
import com.temple.skiaui.compose.core.Shader
import com.temple.skiaui.compose.core.Text
import com.temple.skiaui.compose.core.Video
import com.temple.skiaui.compose.core.View
import com.temple.skiaui.compose.core.runCompose
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setJustifyContent
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setTextSize
import com.temple.skiaui.compose.foundation.setWidth
import com.temple.skiaui.compose.widget.HYComposeView
import kotlin.random.Random

class JetpackComposeTest(val engine: HYSkiaEngine, val context: Long) {

    init {
        engine.registerJetpackCompose()
    }

    fun start(width: Int, height: Int): Long {
        return runCompose({
            var color by remember { mutableStateOf("#ff0000") }
            var shaderSource by remember {
                mutableStateOf(ShaderSource("raining.glsl", arrayOf("raining.png")))
            }
            var epplise by remember {
                mutableStateOf(true)
            }
            var lottiePlay by remember {
                mutableStateOf(true)
            }
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
                Video(
                    modifier = Modifier(context).setSize(width, 360 * width / 640),
                    "yiluxiangbei.mp4"
                )
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
                            epplise = !epplise
                        },
                    content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmojiShow",
                    color = "#0000ff",
                    maxLine = if (epplise) 3 else 0,
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
                ) {
                    Icon(modifier = Modifier(context), 0xe615, color = "#ff0000")
                    Icon(modifier = Modifier(context), 0xe7ce, color = "#ffff00")
                    Icon(modifier = Modifier(context), 0xe670)
                    Icon(modifier = Modifier(context), 0xe67d, color = "#00ff00")
                    Icon(modifier = Modifier(context), 0xe606, color = "#00ffff")
                    Icon(modifier = Modifier(context), 0xe6a2, color = "#000000")
                    Icon(modifier = Modifier(context), 0xe61f)
                }
            }
        }, width, height, engine, context)
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