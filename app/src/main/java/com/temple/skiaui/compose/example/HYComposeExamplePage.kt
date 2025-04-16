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
import androidx.compose.ui.res.stringResource
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
            modifier = Modifier.size(width, height)
                .alignItems(Align.FlexStart),
            "#00000066"
        ) {
            View(
                modifier = Modifier.size(200, 200)
                    .onClick { view: HYComposeView ->
                        color = randomColor()
                    },
                color
            )
            Row(
                modifier = Modifier.width(width)
                    .justifyContent(Justify.SpaceBetween)
                    .alignItems(Align.Center)
                    .margins(intArrayOf(0, 50, 0, 0))
                    .backgroundColor("#ffffff00")
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
                    .backgroundColor("#ffffff00")
            ) {
                SVG(
                    modifier = Modifier.size(480, 480)
                        .backgroundColor("#ffffff00")
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
                    modifier = Modifier.textSize(40)
                        .backgroundColor("#00ffff00"),
                    content = stringResource(R.string.remember_infinite_transition),
                    color = "#ff00ff",
                )
            }
            Text(
                modifier = Modifier
                    .size(800, 100)
                    .textSize(50)
                    .backgroundColor("#ffffff00"),
                content = stringResource(R.string.exo_player),
                color = "#0000ff",
            )
            ExoVideo(
                modifier = Modifier.size(width, 360 * width / 640).corner(60),
                "yiluxiangbei.mp4"
            )
            ComposeNative(width, height)
            ComposeFilament(width, height)
            Loading(
                modifier = Modifier.size(500, 200)
                    .margins(intArrayOf(0, 50, 0, 0))
                    .onClick {
                        color = randomColor()
                    },
                color = color
            )
            Text(
                modifier = Modifier.size(800, 100)
                    .textSize(50)
                    .margins(intArrayOf(0, 50, 0, 0))
                    .onClick {
                        ellipsis = !ellipsis
                    },
                content = "😀😃😄🐦🐋🐟🐡🐴🐊🐄🐪🐘🌸🌏🔥🌟🌚🌝💦💧❄🍕🍔🍟🥝🍱🕶🎩🏈⚽🚴‍♀️🎻🎼🎹🚨🚎🚐⚓🛳🚀🚁🏪🏢🖱⏰📱💾💉📉🛏🔑📁🗓📊❤💯🚫🔻♠♣🕓❗🏳🏁🏳️‍🌈🇮🇹🇱🇷🇺🇸🇬🇧🇨🇳\nEmojiShow",
                color = "#0000ff",
                maxLine = if (ellipsis) 3 else 0,
                ellipsis = stringResource(R.string.click_to_open)
            )
            Lottie(
                modifier = Modifier.size(375, 240)
                    .margins(intArrayOf(0, 50, 0, 0))
                    .onClick {
                        lottiePlay = !lottiePlay
                    },
                "WorkspacePlanet.json",
                lottiePlay
            )
            Shader(
                modifier = Modifier
                    .size(540, 540)
                    .margins(intArrayOf(0, 50, 0, 0))
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
                    .size(300, 300)
                    .margins(intArrayOf(0, 50, 0, 0)), source = "bird.gif"
            )
            Row(
                modifier = Modifier.width(width)
                    .justifyContent(Justify.SpaceBetween)
                    .alignItems(Align.Center)
                    .margins(intArrayOf(0, 50, 0, 0))
                    .backgroundColor("#ffffff00")
            ) {
                Icon(0xe615, color = "#ff0000")
                Icon(0xe7ce, color = "#ffff00")
                Icon(0xe670)
                Icon(0xe67d, color = "#00ff00")
                Icon(0xe606, color = "#00ffff")
                Icon(0xe6a2, color = "#000000")
                Icon(0xe61f)
            }
            Switch(Modifier.margins(intArrayOf(0, 50, 0, 0)))
        }
    }

    @Composable
    private fun ComposeNative(width: Int, height: Int) {
        Button(
            modifier = Modifier.textSize(60)
                .margins(intArrayOf(0, 50, 0, 0))
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
            modifier = Modifier.textSize(60)
                .margins(intArrayOf(0, 50, 0, 0))
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
