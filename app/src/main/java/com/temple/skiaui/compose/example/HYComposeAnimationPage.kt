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
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.HYComposeSDK
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.runtime.View
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.onClick
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setMargins
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.foundation.setTextSize

class HYComposeAnimationPage(engine: HYSkiaEngine, context: Long) :
    HYComposeBasePage(engine, context) {

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
            Text(
                modifier = Modifier(context)
                    .setBackgroundColor("#ffffff00")
                    .setMargins(intArrayOf(0, 50, 0, 0))
                    .setTextSize(50),
                "测试文本",
                convertColor(textColor.value)
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
