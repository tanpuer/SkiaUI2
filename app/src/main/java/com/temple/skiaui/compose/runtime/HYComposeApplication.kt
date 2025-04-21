package com.temple.skiaui.compose.runtime

import androidx.compose.ui.unit.Dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.ui.util.px2dp

abstract class HYComposeApplication(val engine: HYSkiaEngine) {

    open fun onCreate(width: Dp, height: Dp) {

    }

    open fun onDestroy() {

    }

    private fun innerOnCreate(width: Int, height: Int) {
        onCreate(px2dp(width), px2dp(height))
    }

    private fun innerOnDestroy() {
        onDestroy()
    }

}
