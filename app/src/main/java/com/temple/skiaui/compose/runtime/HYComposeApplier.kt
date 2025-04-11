package com.temple.skiaui.compose.runtime

import android.util.Log
import androidx.compose.runtime.AbstractApplier
import com.temple.skiaui.compose.ui.HYComposeNode

class HYComposeApplier(root: HYComposeNode) : AbstractApplier<HYComposeNode>(root) {

    companion object {
        private const val TAG = "HYComposeApplier"
    }

    override fun insertBottomUp(index: Int, instance: HYComposeNode) {
        //ignore: An applier should insert the node into the tree either in [insertTopDown] or [insertBottomUp], not both
    }

    override fun insertTopDown(index: Int, instance: HYComposeNode) {
        Log.d(
            TAG,
            "insertTopDown index:${index} instance:${instance.getViewType()}} parent:${current.getViewType()}"
        )
        current.addElement(index, instance.ref, current.ref)
    }

    override fun remove(index: Int, count: Int) {
        Log.d(TAG, "remove index:${index} count:${count} parent:${current.getViewType()}")
        current.removeElement(index, count)
    }

    override fun move(from: Int, to: Int, count: Int) {
        Log.d(TAG, "move from:${from} to:${to} count:${count} current:${current.getViewType()}")
        current.move(current.ref, from, to, count)
    }

    override fun onClear() {
        Log.d(TAG, "onClear current:${current.getViewType()}")
        current.clear()
    }

}