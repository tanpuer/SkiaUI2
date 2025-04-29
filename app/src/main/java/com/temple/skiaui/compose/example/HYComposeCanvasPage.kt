package com.temple.skiaui.compose.example

import android.util.Log
import androidx.compose.animation.animateColor
import androidx.compose.animation.core.RepeatMode
import androidx.compose.animation.core.infiniteRepeatable
import androidx.compose.animation.core.rememberInfiniteTransition
import androidx.compose.animation.core.tween
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.Canvas
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.runtime.rememberAutoReleaseBitmap
import com.temple.skiaui.compose.runtime.rememberAutoReleasePaint
import com.temple.skiaui.compose.runtime.rememberAutoReleasePath
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.Canvas
import com.temple.skiaui.compose.ui.util.dp2pxf

class HYComposeCanvasPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            val paint = rememberAutoReleasePaint()
            val bitmap = rememberAutoReleaseBitmap(R.drawable.round_logo, 100)
            val path = rememberAutoReleasePath().apply {
                moveTo(100f, dp2pxf(height) / 2 + 100)
                lineTo(400f, dp2pxf(height) / 2 + 100)
                lineTo(250f, dp2pxf(height) / 2 + 300)
                close()
            }
            val rectColor = rememberInfiniteTransition().animateColor(
                initialValue = Color.Yellow,
                targetValue = Color.Cyan,
                animationSpec = infiniteRepeatable(tween(2000), RepeatMode.Reverse),
            )
            val textColor = MaterialTheme.colorScheme.error
            val circleColor = MaterialTheme.colorScheme.inversePrimary
            val lineColor = MaterialTheme.colorScheme.onPrimaryContainer
            DisposableEffect(Unit) {
                onDispose {
                    Log.d("CanvasPage", "onDispose")
                }
            }
            LazyColumn(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                Canvas(
                    modifier = Modifier().size(width, height),
                    onDraw = { canvas: Canvas ->
                        Log.d("CanvasPage", "onDraw")
                        paint.setColor(rectColor.value)
                        canvas.drawRect(100f, 100f, dp2pxf(width) / 2, dp2pxf(height) / 2, paint)
                        paint.setColor(circleColor)
                        canvas.drawCircle(dp2pxf(width) / 2, dp2pxf(height) / 2, 100.0f, paint)
                        canvas.drawText("drawText test", 500f, 500f, dp2pxf(30.dp), textColor)
                        canvas.drawBitmap(
                            bitmap,
                            dp2pxf(width) / 2 + 100,
                            dp2pxf(height) / 2 + 100,
                            null
                        )
                        paint.setColor(lineColor)
                        canvas.drawPath(path, paint)
                        canvas.invalidate()
                    }
                )
            }
        }
    }
}