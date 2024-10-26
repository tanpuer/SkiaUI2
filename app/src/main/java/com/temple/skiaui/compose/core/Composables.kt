package com.temple.skiaui.compose.core

import android.util.Log
import android.view.Choreographer
import androidx.compose.runtime.BroadcastFrameClock
import androidx.compose.runtime.Composable
import androidx.compose.runtime.ComposeNode
import androidx.compose.runtime.ControlledComposition
import androidx.compose.runtime.Recomposer
import androidx.compose.runtime.snapshots.Snapshot
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.widget.HYComposeFlexboxLayout
import com.temple.skiaui.compose.widget.HYComposeLoadingView
import com.temple.skiaui.compose.widget.HYComposePage
import com.temple.skiaui.compose.widget.HYComposeVideo
import com.temple.skiaui.compose.widget.HYComposeView
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.CoroutineStart
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlin.coroutines.CoroutineContext

@Composable
fun View(modifier: Modifier, backgroundColor: String) {
    ComposeNode<HYComposeView, HYComposeApplier>(
        factory = { HYComposeView(modifier) },
        update = {
            set(backgroundColor) {
                this.setBackgroundColor(backgroundColor)
            }
        }
    )
}

@Composable
fun Column(modifier: Modifier, content: @Composable () -> Unit) {
    ComposeNode<HYComposeFlexboxLayout, HYComposeApplier>(
        factory = { HYComposeFlexboxLayout(modifier) },
        update = {},
        content = content
    )
}

@Composable
fun Video(modifier: Modifier) {
    ComposeNode<HYComposeVideo, HYComposeApplier>(
        factory = { HYComposeVideo(modifier) },
        update = {}
    )
}

@Composable
fun Page(modifier: Modifier) {
    ComposeNode<HYComposePage, HYComposeApplier>(
        factory = { HYComposePage(modifier) },
        update = {}
    )
}

@Composable
fun Loading(modifier: Modifier) {
    ComposeNode<HYComposeLoadingView, HYComposeApplier>(
        factory = { HYComposeLoadingView(modifier) },
        update = {}
    )
}

fun runCompose(
    content: @Composable () -> Unit,
    width: Int,
    height: Int,
    engine: HYSkiaEngine,
    context: Long
): Long {
    val frameClock = BroadcastFrameClock()
    val reComposer = Recomposer(frameClock)
    val rootNode = HYComposePage(
        Modifier(context)
            .setSize(width, height)
            .setBackgroundColor("#ffffffff")
    )
    rootNode.view
    val composition = ControlledComposition(
        applier = HYComposeApplier(rootNode),
        parent = reComposer
    )
    composition.setContent(content)
    val coroutineContext: CoroutineContext by lazy {
        val dispatcher = HYComposeUIDispatcher(
            Choreographer.getInstance(),
            engine
        )
        dispatcher + dispatcher.frameClock
    }
    val scope = CoroutineScope(coroutineContext)
    scope.launch {
        var snapshotJob: Job? = null
        val snapshotHandle = Snapshot.registerGlobalWriteObserver {
            if (snapshotJob == null) {
                snapshotJob = scope.launch {
                    snapshotJob = null
                    Snapshot.sendApplyNotifications()
                }
            }
        }
        val recomposeJob = scope.launch(start = CoroutineStart.UNDISPATCHED) {
            Log.d(
                "runRecomposeAndApplyChanges",
                "runRecomposeAndApplyChanges ${Thread.currentThread()}"
            )
            reComposer.runRecomposeAndApplyChanges()
        }
    }
    return rootNode.ref
}