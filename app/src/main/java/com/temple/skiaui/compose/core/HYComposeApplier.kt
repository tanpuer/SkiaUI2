package com.temple.skiaui.compose.core

import androidx.compose.runtime.AbstractApplier
import com.temple.skiaui.compose.widget.HYComposeNode

class HYComposeApplier(root: HYComposeNode) : AbstractApplier<HYComposeNode>(root) {

    override fun insertBottomUp(index: Int, instance: HYComposeNode) {
        //ignore
    }

    override fun insertTopDown(index: Int, instance: HYComposeNode) {
        current.addElement(index, instance.view.ref, current.view.ref)
    }

    override fun remove(index: Int, count: Int) {
        current.removeElement(index, count, current.view.ref)
    }

    override fun move(from: Int, to: Int, count: Int) {
        current.move(from, to, count)
    }

    override fun onClear() {
        current.clear()
    }

}