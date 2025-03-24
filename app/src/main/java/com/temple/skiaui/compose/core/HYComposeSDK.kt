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
import java.util.Stack
import kotlin.coroutines.CoroutineContext

object HYComposeSDK {

    private const val TAG = "HYComposeSDK"

    private val frameClock = BroadcastFrameClock()

    private lateinit var reComposer: Recomposer

    private lateinit var pageStack: Stack<HYComposeBasePage>

    fun initSDK(engine: HYSkiaEngine) {
        reComposer = Recomposer(frameClock)
        pageStack = Stack<HYComposeBasePage>()
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

    fun DeInitSDK() {
        reComposer.close()
        pageStack.clear()
    }

    fun getRecomposer(): Recomposer {
        return reComposer
    }

    fun pushPage(page: HYComposeBasePage) {
        Log.d(TAG, "pushPage")
        pageStack.push(page)
    }

    fun popPage(engine: HYSkiaEngine) {
        engine.onBackPressed()
    }

    fun onPagePoped() {
        if (pageStack.empty()) {
            Log.e(TAG, "popPage failed")
            return
        }
        val page = pageStack.pop()
        page.dispose()
        if (pageStack.empty()) {
            DeInitSDK()
        }
    }

}
