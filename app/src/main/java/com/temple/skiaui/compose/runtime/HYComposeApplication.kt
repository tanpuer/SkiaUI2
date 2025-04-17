package com.temple.skiaui.compose.runtime

import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp

abstract class HYComposeApplication(val engine: HYSkiaEngine) {

    open fun onCreate(width: Dp, height: Dp) {

    }

    open fun onDestroy() {

    }

    private fun innerOnCreate(width: Int, height: Int) {
        val density = HYSkiaUIApp.getInstance().resources.displayMetrics.density
        onCreate((width / density).dp, (height / density).dp)
    }

    private fun innerOnDestroy() {
        onDestroy()
    }

}
