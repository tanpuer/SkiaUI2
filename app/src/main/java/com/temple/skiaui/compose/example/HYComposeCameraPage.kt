package com.temple.skiaui.compose.example

import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.Button
import com.temple.skiaui.compose.runtime.Camera
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.CameraCallback

class HYComposeCameraPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            var shaderPath by remember {
                mutableStateOf("")
            }
            Scroll(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                Camera(
                    modifier = Modifier().size(width, width),
                    callback = object : CameraCallback {
                        override fun onImageCaptured(imagePtr: Long) {

                        }
                    },
                    shaderPath = shaderPath
                )
                Button(
                    modifier = Modifier
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
                    content = stringResource(R.string.video_black_white_shader),
                    textSize = 20.dp,
                    color = MaterialTheme.colorScheme.tertiary,
                    onClick = {
                        shaderPath = "skia_video_black_white.glsl"
                    }
                )
                Button(
                    modifier = Modifier
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
                    content = stringResource(R.string.video_lightning_shader),
                    textSize = 20.dp,
                    color = MaterialTheme.colorScheme.tertiary,
                    onClick = {
                        shaderPath = "skia_video_lightning.glsl"
                    }
                )
                Button(
                    modifier = Modifier
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .backgroundColor(MaterialTheme.colorScheme.tertiaryContainer),
                    content = stringResource(R.string.video_raining_shader),
                    textSize = 20.dp,
                    color = MaterialTheme.colorScheme.tertiary,
                    onClick = {
                        shaderPath = "skia_video_raining_shader.glsl"
                    }
                )
            }
        }
    }
}