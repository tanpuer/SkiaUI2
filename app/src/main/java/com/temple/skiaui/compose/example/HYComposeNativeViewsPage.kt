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
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
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
    override fun RunComposable(width: Dp, height: Dp) {
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
            colorResource(R.color.gray_bg)
        ) {
            EditText(
                modifier = Modifier.size(width, 70.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)),
                stringResource(R.string.edit_hint_string),
                true
            )
            Camera(Modifier().size(width, width), object : CameraCallback {
                override fun onImageCaptured(imagePtr: Long) {

                }
            })
            Web(
                Modifier.size(width, 1200.dp)
                    .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)),
                "https://m.bilibili.com/"
            )
        }
    }

    companion object {
        private const val TAG = "HYComposeAnimationPage"
    }
}
