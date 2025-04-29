package com.temple.skiaui.compose.example

import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.unit.Dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.Camera
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.CameraCallback

class HYComposeCameraPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            Scroll(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                Camera(Modifier().size(width, width), object : CameraCallback {
                    override fun onImageCaptured(imagePtr: Long) {

                    }
                })
            }
        }
    }
}