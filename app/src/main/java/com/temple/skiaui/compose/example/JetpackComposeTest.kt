package com.temple.skiaui.compose.example

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.Column
import com.temple.skiaui.compose.core.Video
import com.temple.skiaui.compose.core.View
import com.temple.skiaui.compose.core.runCompose
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setSource

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
            ) {
                View(
                    modifier = Modifier(context).setSize(200, 200), color
                )
                Video(
                    modifier = Modifier(context).setSize(width, 360 * width / 640).setSource("yiluxiangbei.mp4")
                )
            }
        }, width, height, engine, context)
    }

}