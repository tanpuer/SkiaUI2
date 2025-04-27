package com.temple.skiaui.compose.example

import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.Canvas
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.Canvas
import com.temple.skiaui.compose.ui.HYComposePaint
import com.temple.skiaui.compose.ui.util.dp2pxf

class HYComposeCanvasPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            val paint by remember {
                mutableStateOf(HYComposePaint())
            }
            paint.setColor(MaterialTheme.colorScheme.primary)
            LazyColumn(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            )
            {
                Canvas(
                    modifier = Modifier().size(width, height),
                    onDraw = { canvas: Canvas ->
                        canvas.drawRect(100f, 100f, dp2pxf(width) / 2, dp2pxf(height) / 2, paint)
                    }
                )
            }
        }
    }
}