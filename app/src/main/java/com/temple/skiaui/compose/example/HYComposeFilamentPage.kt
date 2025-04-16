package com.temple.skiaui.compose.example

import androidx.compose.runtime.Composable
import androidx.compose.ui.res.colorResource
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.runtime.Filament
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.LazyColumn
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.ui.Align

class HYComposeFilamentPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Int, height: Int) {
        LazyColumn(
            modifier = Modifier().size(width, height)
                .alignItems(Align.FlexStart),
            colorResource(R.color.transparent)
        ) {
            Filament(modifier = Modifier().size(width, 500))
        }
    }
}