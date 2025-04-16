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
import androidx.compose.ui.res.colorResource
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.HYComposeSDK
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.runtime.View
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.foundation.textSize
import com.temple.skiaui.compose.ui.Align

class HYComposeMaterialPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

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
            colorResource(R.color.transparent)
        ) {
            View(
                modifier = Modifier()
                    .size(200, 200)
                    .onClick {
                        HYComposeSDK.popPage(engine)
                    },
                color.value
            )
            Text(
                modifier = Modifier()
                    .backgroundColor(colorResource(R.color.transparent))
                    .margins(intArrayOf(0, 50, 0, 0))
                    .textSize(50),
                "测试文本",
                textColor.value
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
