package com.temple.skiaui.compose.runtime

import android.util.Log
import androidx.compose.runtime.Composable
import androidx.compose.runtime.CompositionLocalProvider
import androidx.compose.runtime.ControlledComposition
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.Dp
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.ui.HYComposePage
import com.temple.skiaui.compose.ui.util.dp2px
import com.temple.skiaui.compose.ui.util.px2dp

abstract class HYComposeBasePage(val engine: HYSkiaEngine) : HYComposeComposer(engine) {

    private val pageIndex = "${INDEX++}"

    open fun start(width: Dp, height: Dp) {
        composition = ControlledComposition(
            applier = HYComposeApplier(
                HYComposePage(
                    modifier = Modifier().size(width, height),
                    onShow = { this.onShow() },
                    onHide = { this.onHide() }
                ).apply {
                    this.push(dp2px(modifier.width))
                }),
            parent = reComposer
        )
        composition?.setContent {
            CompositionLocalProvider(
                LocalContext provides engine.getContext(),
                LocalConfiguration provides engine.getContext().resources.configuration,
                LocalLifecycleOwner provides engine.getContext() as LifecycleOwner,
            ) {
                val composeWidth = remember { mutableStateOf(width) }
                val composeHeight = remember { mutableStateOf(height) }
                DisposableEffect(Unit) {
                    engine.addSizeChangeListener(pageIndex) { viewWidth, viewHeight ->
                        composeWidth.value = px2dp(viewWidth)
                        composeHeight.value = px2dp(viewHeight)
                    }
                    onDispose {
                        engine.removeSizeChangeListener(pageIndex)
                    }
                }
                RunComposable(composeWidth.value, composeHeight.value)
            }
        }
    }

    @Composable
    abstract fun RunComposable(width: Dp, height: Dp)

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

    open fun onUIModeChange() {
        Log.d(TAG, "onUIModeChange")
        composition?.invalidateAll()
    }

    companion object {
        private const val TAG = "HYComposeBasePage"
        private var INDEX = 0
    }

}