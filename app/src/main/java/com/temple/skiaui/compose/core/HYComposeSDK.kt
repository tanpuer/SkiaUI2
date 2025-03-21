package com.temple.skiaui.compose.core

import android.util.Log
import android.view.Choreographer
import androidx.compose.runtime.BroadcastFrameClock
import androidx.compose.runtime.Recomposer
import androidx.compose.runtime.snapshots.Snapshot
import com.temple.skiaui.HYSkiaEngine
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.CoroutineStart
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlin.coroutines.CoroutineContext

object HYComposeSDK {

    private val frameClock = BroadcastFrameClock()

    val reComposer = Recomposer(frameClock)

    fun initSDK(engine: HYSkiaEngine) {
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