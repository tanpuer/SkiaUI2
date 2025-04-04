package com.temple.skiaui.compose.core

import com.temple.skiaui.HYSkiaEngine

abstract class HYComposeApplication(val engine: HYSkiaEngine, val context: Long) {

    open fun onCreate(width: Int, height: Int) {

    }

    open fun onDestroy() {

    }

}
