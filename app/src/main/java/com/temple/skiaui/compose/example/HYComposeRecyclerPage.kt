package com.temple.skiaui.compose.example

import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.RecyclerView
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.HYComposeText
import com.temple.skiaui.compose.ui.HYComposeView
import com.temple.skiaui.compose.ui.TextAlign

class HYComposeRecyclerPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            RecyclerView(
                modifier = Modifier.size(width, height).alignItems(Align.Center),
                backgroundColor = MaterialTheme.colorScheme.background,
                createView = { index ->
                    HYComposeText(modifier = Modifier.width(width)).apply {
                        setTextSize(50.dp)
                        setBackgroundColor(Color.Transparent)
                        setTextAlign(TextAlign.kCenter)
                    }
                },
                bindView = { index: Int, view: HYComposeView ->
                    val textNode = view as HYComposeText
                    textNode.setText("$index")
                },
                viewType = { index -> 0 },
                dataSize = 100
            )
        }
    }


}