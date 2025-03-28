package com.temple.skiaui.compose.example

import androidx.compose.runtime.Composable
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.Camera
import com.temple.skiaui.compose.core.HYComposeBasePage
import com.temple.skiaui.compose.core.LazyColumn
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.widget.CameraCallback

class HYComposeCameraPage(engine: HYSkiaEngine, context: Long) :
    HYComposeBasePage(engine, context) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
        LazyColumn(
            modifier = Modifier(context).setSize(width, height)
                .setAlignItems("flex-start"),
            "#ffffff"
        ) {
            Camera(Modifier(context).setSize(width, width), object : CameraCallback {
                override fun onImageCaptured(imagePtr: Long) {

                }
            })
        }
    }
}