package com.temple.skiaui.compose.example

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.Column
import com.temple.skiaui.compose.core.LazyColumn
import com.temple.skiaui.compose.core.Loading
import com.temple.skiaui.compose.core.Lottie
import com.temple.skiaui.compose.core.Shader
import com.temple.skiaui.compose.core.Text
import com.temple.skiaui.compose.core.Video
import com.temple.skiaui.compose.core.View
import com.temple.skiaui.compose.core.runCompose
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setSource
import com.temple.skiaui.compose.foundation.setTextSize
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
            LazyColumn(
                modifier = Modifier(context).setSize(width, height)
                    .setAlignItems("flex-end"),
                "#00000066"
            ) {
                View(
                    modifier = Modifier(context)
                        .setSize(200, 200)
                        .onClick { view: HYComposeView ->
                            val hexChars = "0123456789abcdef"
                            val nextColor = StringBuilder("#")
                            for (i in 0 until 6) {
                                nextColor.append(hexChars[Random.nextInt(hexChars.length)])
                            }
                            color = nextColor.toString()
                        },
                    color
                )
                Video(
                    modifier = Modifier(context).setSize(width, 360 * width / 640)
                        .setSource("yiluxiangbei.mp4")
                )
                Loading(modifier = Modifier(context).setSize(500, 200))
                Text(
                    modifier = Modifier(context)
                        .setSize(800, 100)
                        .setTextSize(50)
                        .onClick {
                            epplise = !epplise
                        },
                    content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmoji展示",
                    color = "#0000ff",
                    maxLine = if (epplise) 3 else 0
                )
                Lottie(
                    modifier = Modifier(context).setSize(375, 240).setSource("WorkspacePlanet.json")
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
            }
        }, width, height, engine, context)
    }

}