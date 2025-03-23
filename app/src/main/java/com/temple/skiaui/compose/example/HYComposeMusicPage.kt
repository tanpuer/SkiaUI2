package com.temple.skiaui.compose.example

import androidx.compose.runtime.Composable
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
        Page(modifier = Modifier(context).setSize(width, height)) {
            LazyColumn(
                modifier = Modifier(context).setSize(width, height)
                    .setAlignItems("flex-start"),
                "#00000066"
            ) {
                View(
                    modifier = Modifier(context)
                        .setSize(200, 200)
                        .onClick {
                            HYComposeSDK.popPage(engine)
                        },
                    "#ff0000"
                )

            }
        }
    }
}
