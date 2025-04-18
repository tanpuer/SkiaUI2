package com.temple.skiaui.compose.example

import android.util.Log
import androidx.compose.animation.Animatable
import androidx.compose.animation.animateColor
import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.HYComposeSDK
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.runtime.Text
import com.temple.skiaui.compose.runtime.View
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.foundation.textSize
import com.temple.skiaui.compose.ui.Align

class HYComposeMaterialPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
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
                MaterialTheme.colorScheme.background
            ) {
                View(
                    modifier = Modifier()
                        .size(100.dp, 100.dp),
                    color.value,
                    onClick = {
                        HYComposeSDK.popPage(engine)
                    }
                )
                Text(
                    modifier = Modifier()
                        .backgroundColor(Color.Transparent)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp))
                        .textSize(50.dp),
                    stringResource(R.string.test_text),
                    textColor.value
                )
            }
        }
    }

    companion object {
        private const val TAG = "HYComposeAnimationPage"
    }
}
