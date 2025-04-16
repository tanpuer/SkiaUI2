package com.temple.skiaui.compose.runtime

import android.util.Log
import androidx.compose.runtime.Composable
import androidx.compose.runtime.CompositionLocalProvider
import androidx.compose.runtime.ControlledComposition
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.ui.HYComposePage

abstract class HYComposeBasePage(val engine: HYSkiaEngine) : HYComposeComposer(engine) {

    open fun start(width: Int, height: Int) {
        composition = ControlledComposition(
            applier = HYComposeApplier(
                HYComposePage(
                    modifier = Modifier().size(width, height),
                    onShow = { this.onShow() },
                    onHide = { this.onHide() }
                ).apply {
                    this.push((modifier.styles["size"] as IntArray)[0])
                }),
            parent = reComposer
        )
        composition?.setContent {
            CompositionLocalProvider(
                LocalContext provides engine.getContext(),
                LocalConfiguration provides engine.getContext().resources.configuration,
                LocalLifecycleOwner provides engine.getContext() as LifecycleOwner
            ) {
                RunComposable(width, height)
            }
        }
    }

    @Composable
    abstract fun RunComposable(width: Int, height: Int)

    open fun dispose() {
        Log.d(TAG, "dispose")
        composition?.dispose()
        reComposer.close()
    }

    open fun onShow() {
        Log.d(TAG, "onShow")
        reComposer.resumeCompositionFrameClock()
    }

    open fun onHide() {
        Log.d(TAG, "onHide")
        reComposer.pauseCompositionFrameClock()
    }

    companion object {
        private const val TAG = "HYComposeBasePage"
    }

}