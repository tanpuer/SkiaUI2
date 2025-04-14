package com.temple.skiaui.compose.example

import android.util.Log
import androidx.compose.animation.Animatable
import androidx.compose.animation.animateColor
import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.Color
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.Camera
import com.temple.skiaui.compose.runtime.EditText
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.runtime.Web
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.CameraCallback

class HYComposeNativeViewsPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
        val color = remember { Animatable(Color.Green) }
        LaunchedEffect(color) {
            color.animateTo(Color.Blue, infiniteRepeatable(tween(5000), RepeatMode.Reverse))
        }
        val textColor = rememberInfiniteTransition().animateColor(
            initialValue = Color(0xFF60DDAD),
            targetValue = Color(0xFF4285F4),
            animationSpec = infiniteRepeatable(tween(1000), RepeatMode.Reverse),
        )
        DisposableEffect(engine.getContext()) {
            onDispose {
                Log.d(TAG, "onDispose")
            }
        }
        LazyColumn(
            modifier = Modifier().size(width, height)
                .alignItems(Align.Center),
            "#00000066"
        ) {
            EditText(
                modifier = Modifier.size(width, 150)
                    .margins(intArrayOf(0, 50, 0, 0)),
                "这是Android原生的EditText融合渲染",
                true
            )
            Camera(Modifier().size(width, width), object : CameraCallback {
                override fun onImageCaptured(imagePtr: Long) {

                }
            })
            Web(
                Modifier.size(width, 1200)
                    .margins(intArrayOf(0, 50, 0, 0)),
                "https://m.bilibili.com/"
            )
        }
    }

    private fun convertColor(color: Color): String {
        val r = (color.red * 255).toInt()
        val g = (color.green * 255).toInt()
        val b = (color.blue * 255).toInt()
        val a = (color.alpha * 255).toInt()
        return String.format("#%02X%02X%02X%02X", r, g, b, a)
    }

    companion object {
        private const val TAG = "HYComposeAnimationPage"
    }
}
