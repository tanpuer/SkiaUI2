package com.temple.skiaui.compose.core

import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.compose.runtime.AbstractApplier
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.compose.widget.HYComposeNode

class HYComposeApplier(root: HYComposeNode) : AbstractApplier<HYComposeNode>(root) {

    companion object {
        private const val TAG = "HYComposeApplier"
    }

    private val handler = Handler(Looper.myLooper()!!)
    private var runnableList = arrayListOf<Runnable>()
    private var addImmediately = true
    private val delay = 1000L / HYSkiaUIApp.getInstance().getFrameRate()

    init {
        Log.d(TAG, "addImmediately = ${addImmediately}!")
    }

    private val addRunnable = object : Runnable {
        override fun run() {
            if (runnableList.isEmpty()) {
                return
            }
            runnableList.removeAt(0).run()
            if (runnableList.isNotEmpty()) {
                handler.postDelayed(this, delay)
            } else {
                addImmediately = true
                Log.d(TAG, "addImmediately = ${addImmediately}!")
            }
        }
    }

    override fun insertBottomUp(index: Int, instance: HYComposeNode) {

    }

    override fun insertTopDown(index: Int, instance: HYComposeNode) {
        if (addImmediately) {
            current.addElement(index, instance.ref, current.ref)
            return
        }
        val aa = current
        val bb = instance.ref
        if (runnableList.isEmpty()) {
            handler.postDelayed(addRunnable, delay)
        }
        runnableList.add { aa.addElement(index, bb, aa.ref) }
    }

    override fun remove(index: Int, count: Int) {
        current.removeElement(index, count)
    }

    override fun move(from: Int, to: Int, count: Int) {
        current.move(from, to, count)
    }

    override fun onClear() {
        current.clear()
    }

}