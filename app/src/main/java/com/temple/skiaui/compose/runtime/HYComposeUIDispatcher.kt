package com.temple.skiaui.compose.runtime

import android.os.Handler
import android.os.Looper
import android.view.Choreographer
import androidx.compose.runtime.MonotonicFrameClock
import com.temple.skiaui.HYSkiaEngine
import kotlinx.coroutines.CoroutineDispatcher
import kotlin.coroutines.CoroutineContext

/**
 * A [CoroutineDispatcher] that will perform dispatch during a [handler] callback or
 * [choreographer]'s animation frame stage, whichever comes first. Use [Main] to obtain
 * a dispatcher for the process's main thread (i.e. the activity thread).
 */
// Implementation note: the constructor is private to direct users toward the companion object
// accessors for the main/current threads. A choreographer must be obtained from its current
// thread as per the only public API surface for obtaining one as of this writing, and the
// choreographer and handler must match. Constructing an AndroidUiDispatcher with a handler
// not marked as async will adversely affect dispatch behavior but not to the point of
// incorrectness; more operations would be deferred to the choreographer frame as racing handler
// messages would wait behind a frame barrier.
class HYComposeUIDispatcher(
    val choreographer: Choreographer, val engine: HYSkiaEngine
) : CoroutineDispatcher() {

    // Guards all properties in this class
    private val lock = Any()

    private val toRunTrampolined = ArrayDeque<Runnable>()
    private var toRunOnFrame = mutableListOf<Choreographer.FrameCallback>()
    private var spareToRunOnFrame = mutableListOf<Choreographer.FrameCallback>()
    private var scheduledTrampolineDispatch = false
    private var scheduledFrameDispatch = false
    private val mainHandler = engine.mainHandler

    private val dispatchCallback = object : Choreographer.FrameCallback, Runnable {
        override fun run() {
            performTrampolineDispatch()
            synchronized(lock) {
                if (toRunOnFrame.isEmpty()) {
                    choreographer.removeFrameCallback(this)
                    scheduledFrameDispatch = false
                }
            }
        }

        override fun doFrame(frameTimeNanos: Long) {
            engine.postToSkiaUI {
                performTrampolineDispatch()
                performFrameDispatch(frameTimeNanos)
            }
        }
    }

    private fun nextTask(): Runnable? = synchronized(lock) {
        toRunTrampolined.removeFirstOrNull()
    }

    private fun performTrampolineDispatch() {
        do {
            var task = nextTask()
            while (task != null) {
                task.run()
                task = nextTask()
            }
        } while (
        // We don't dispatch holding the lock so that other tasks can get in on our
        // trampolining time slice, but once we're done, make sure nothing added a new task
        // before we set scheduledDispatch = false, which would prevent the next dispatch
        // from being correctly scheduled. Loop to run these stragglers now.
            synchronized(lock) {
                if (toRunTrampolined.isEmpty()) {
                    scheduledTrampolineDispatch = false
                    false
                } else true
            }
        )
    }

    private fun performFrameDispatch(frameTimeNanos: Long) {
        val toRun = synchronized(lock) {
            if (!scheduledFrameDispatch) return
            scheduledFrameDispatch = false
            val result = toRunOnFrame
            toRunOnFrame = spareToRunOnFrame
            spareToRunOnFrame = result
            result
        }
        for (i in 0 until toRun.size) {
            // This callback will not and must not throw, see AndroidUiFrameClock
            toRun[i].doFrame(frameTimeNanos)
        }
        toRun.clear()
    }

    internal fun postFrameCallback(callback: Choreographer.FrameCallback) {
        synchronized(lock) {
            toRunOnFrame.add(callback)
            if (!scheduledFrameDispatch) {
                scheduledFrameDispatch = true
                choreographer.postFrameCallback(dispatchCallback)
            }
        }
    }

    internal fun removeFrameCallback(callback: Choreographer.FrameCallback) {
        synchronized(lock) {
            toRunOnFrame.remove(callback)
        }
    }

    /**
     * A [MonotonicFrameClock] associated with this [AndroidUiDispatcher]'s [choreographer]
     * that may be used to await [Choreographer] frame dispatch.
     */
    public val frameClock: MonotonicFrameClock = HYComposeUIFrameClock(choreographer)

    override fun dispatch(context: CoroutineContext, block: Runnable) {
        synchronized(lock) {
            toRunTrampolined.addLast(block)
            if (!scheduledTrampolineDispatch) {
                scheduledTrampolineDispatch = true
                engine.postToSkiaUI {
                    dispatchCallback.run()
                }
                if (!scheduledFrameDispatch) {
                    scheduledFrameDispatch = true
                    mainHandler.post {
                        choreographer.postFrameCallback(dispatchCallback)
                    }
                }
            }
        }
    }

}