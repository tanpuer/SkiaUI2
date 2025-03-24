package com.temple.skiaui.compose.example

import androidx.compose.animation.Animatable
import androidx.compose.animation.core.InfiniteRepeatableSpec
import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.tween
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.Color
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.HYComposeBasePage
import com.temple.skiaui.compose.core.HYComposeSDK
import com.temple.skiaui.compose.core.LazyColumn
import com.temple.skiaui.compose.core.Page
import com.temple.skiaui.compose.core.View
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setSize

class HYComposeMusicPage(engine: HYSkiaEngine, context: Long) :
    HYComposeBasePage(engine, context) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
        val color = remember { Animatable(Color.Green) }
        LaunchedEffect(color) {
            color.animateTo(Color.Blue, infiniteRepeatable(tween(5000), RepeatMode.Reverse))
        }
        Page(modifier = Modifier(context).setSize(width, height)) {
            LazyColumn(
                modifier = Modifier(context).setSize(width, height)
                    .setAlignItems("center"),
                "#00000066"
            ) {
                View(
                    modifier = Modifier(context)
                        .setSize(200, 200)
                        .onClick {
                            HYComposeSDK.popPage(engine)
                        },
                    convertColor(color.value)
                )

            }
        }
    }

    private fun convertColor(color: Color): String {
        val r = (color.red * 255).toInt()
        val g = (color.green * 255).toInt()
        val b = (color.blue * 255).toInt()
        val a = (color.alpha * 255).toInt()
        return String.format("#%02X%02X%02X%02X", r, g, b, a)
    }
}
