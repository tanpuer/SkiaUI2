package com.temple.skiaui.compose.example

import androidx.compose.runtime.Composable
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.runtime.Filament
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setAlignItems
import com.temple.skiaui.compose.foundation.setSize

class HYComposeFilamentPage(engine: HYSkiaEngine, context: Long) :
    HYComposeBasePage(engine, context) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
        LazyColumn(
            modifier = Modifier(context).setSize(width, height)
                .setAlignItems("flex-start"),
            "#ffffff"
        ) {
            Filament(modifier = Modifier(context).setSize(width, 500))
        }
    }
}