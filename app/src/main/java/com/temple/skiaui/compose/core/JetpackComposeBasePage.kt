package com.temple.skiaui.compose.core

import androidx.compose.runtime.Composable
import androidx.compose.runtime.ControlledComposition
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.widget.HYComposeRoot

abstract class JetpackComposeBasePage(val engine: HYSkiaEngine, val context: Long) {

    companion object {
        var registerFlag = false
    }

    init {
        if (!registerFlag) {
            engine.registerJetpackCompose()
            JetPackComposeSDK.initSDK(engine)
            registerFlag = true
        }
    }

    @Composable
    abstract fun RunComposable(width: Int, height: Int)

    /**
     * called from JNI
     */
    private fun start(width: Int, height: Int) {
        val composition = ControlledComposition(
            applier = HYComposeApplier(HYComposeRoot(Modifier(context))),
            parent = JetPackComposeSDK.reComposer
        )
        composition.setContent {
            RunComposable(width, height)
        }
    }

}