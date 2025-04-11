package com.temple.skiaui.compose.runtime

import androidx.compose.runtime.Composable
import androidx.compose.runtime.ControlledComposition
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.ui.HYComposePage

abstract class HYComposeBasePage(val engine: HYSkiaEngine) {

    private var composition: ControlledComposition? = null

    @Composable
    abstract fun RunComposable(width: Int, height: Int)

    open fun start(width: Int, height: Int) {
        composition = ControlledComposition(
            applier = HYComposeApplier(
                HYComposePage(
                    Modifier().setSize(width, height)
                ).apply {
                    this.push((modifier.styles["size"] as IntArray)[0])
                }),
            parent = HYComposeSDK.getRecomposer()
        )
        composition?.setContent {
            RunComposable(width, height)
        }
    }

    open fun dispose() {
        composition?.dispose()
    }

}