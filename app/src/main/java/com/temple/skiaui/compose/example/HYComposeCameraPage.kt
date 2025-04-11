package com.temple.skiaui.compose.example

import androidx.compose.runtime.Composable
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.runtime.Camera
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.ui.CameraCallback

class HYComposeCameraPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
        LazyColumn(
            modifier = Modifier().setSize(width, height)
                .setAlignItems("flex-start"),
            "#ffffff"
        ) {
            Camera(Modifier().setSize(width, width), object : CameraCallback {
                override fun onImageCaptured(imagePtr: Long) {

                }
            })
        }
    }
}