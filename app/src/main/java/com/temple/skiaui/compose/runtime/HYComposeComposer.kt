package com.temple.skiaui.compose.runtime

import android.util.Log
import android.view.Choreographer
import androidx.compose.runtime.BroadcastFrameClock
import androidx.compose.runtime.ControlledComposition
import androidx.compose.runtime.Recomposer
import androidx.compose.runtime.snapshots.Snapshot
import com.temple.skiaui.HYSkiaEngine
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.CoroutineStart
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlin.coroutines.CoroutineContext

abstract class HYComposeComposer(engine: HYSkiaEngine) {

    protected var composition: ControlledComposition? = null

    private val frameClock = BroadcastFrameClock()

    protected val reComposer = Recomposer(frameClock)

    init {
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
    }
}