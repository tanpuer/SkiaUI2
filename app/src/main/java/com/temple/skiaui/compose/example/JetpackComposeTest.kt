package com.temple.skiaui.compose.example

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.Column
import com.temple.skiaui.compose.core.Loading
import com.temple.skiaui.compose.core.Lottie
import com.temple.skiaui.compose.core.Scroll
import com.temple.skiaui.compose.core.Shader
import com.temple.skiaui.compose.core.Text
import com.temple.skiaui.compose.core.Video
import com.temple.skiaui.compose.core.View
import com.temple.skiaui.compose.core.runCompose
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setJustifyContent
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setSource
import com.temple.skiaui.compose.foundation.setTextSize
import com.temple.skiaui.compose.foundation.setWidth

class JetpackComposeTest(val engine: HYSkiaEngine, val context: Long) {

    init {
        engine.registerJetpackCompose()
    }

    fun start(width: Int, height: Int): Long {
        return runCompose({
            var color by remember { mutableStateOf("#ff0000") }
            engine.postToSkiaUIDelay({
                color = "#00ff00"
            }, 2000)
            Column(
                modifier = Modifier(context).setSize(width, height)
                    .setBackgroundColor("#00000066")
                    .setAlignItems("center")
            ) {
                View(
                    modifier = Modifier(context).setSize(200, 200), color
                )
                Video(
                    modifier = Modifier(context).setSize(width, 360 * width / 640)
                        .setSource("yiluxiangbei.mp4")
                )
                Loading(modifier = Modifier(context).setSize(width, 200))
                Lottie(
                    modifier = Modifier(context).setSize(375, 240).setSource("WorkspacePlanet.json")
                )
                Shader(
                    modifier = Modifier(context).setSize(540, 260).setSource("sincos.glsl")
                )
                Text(
                    modifier = Modifier(context).setWidth(800).setTextSize(50),
                    content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄\n🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊\n❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmoji展示"
                )
            }
        }, width, height, engine, context)
    }

}