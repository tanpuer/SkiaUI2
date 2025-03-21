package com.temple.skiaui.compose.core

import androidx.compose.runtime.Composable
import androidx.compose.runtime.ControlledComposition
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.widget.HYComposeRoot

abstract class HYComposeBasePage(val engine: HYSkiaEngine, val context: Long) {

    private var composition: ControlledComposition? = null

    @Composable
    abstract fun RunComposable(width: Int, height: Int)

    fun start(width: Int, height: Int) {
        composition = ControlledComposition(
            applier = HYComposeApplier(HYComposeRoot(Modifier(context))),
            parent = HYComposeSDK.reComposer
        )
        composition?.setContent {
            RunComposable(width, height)
        }
    }

    fun dispose() {
        composition?.dispose()
    }

}