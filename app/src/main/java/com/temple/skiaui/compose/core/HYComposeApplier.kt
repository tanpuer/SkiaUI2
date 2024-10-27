package com.temple.skiaui.compose.core

import androidx.compose.runtime.AbstractApplier
import com.temple.skiaui.compose.widget.HYComposeNode

class HYComposeApplier(root: HYComposeNode) : AbstractApplier<HYComposeNode>(root) {

    override fun insertBottomUp(index: Int, instance: HYComposeNode) {
        //ignore
    }

    override fun insertTopDown(index: Int, instance: HYComposeNode) {
        current.addElement(index, instance.ref, current.ref)
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