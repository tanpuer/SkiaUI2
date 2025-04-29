package com.temple.skiaui.compose.example

import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.runtime.Filament
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.ui.Align

class HYComposeFilamentPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            Scroll(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.FlexStart),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                Filament(modifier = Modifier().size(width, 400.dp))
            }
        }
    }
}