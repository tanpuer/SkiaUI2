package com.temple.skiaui.compose.core

import androidx.compose.runtime.AbstractApplier
import com.temple.skiaui.compose.widget.HYComposeNode
import com.temple.skiaui.compose.widget.HYComposeRoot

class HYComposeApplier(root: HYComposeNode) : AbstractApplier<HYComposeNode>(root) {

    override fun insertBottomUp(index: Int, instance: HYComposeNode) {
        if (instance is HYComposeRoot) {
            return
        }
    }

    override fun insertTopDown(index: Int, instance: HYComposeNode) {
        if (current is HYComposeRoot) {
            return
        }
        current.addElement(index, instance.ref, current.ref)
    }

    override fun remove(index: Int, count: Int) {
        if (current is HYComposeRoot) {
            return
        }
        current.removeElement(index, count)
    }

    override fun move(from: Int, to: Int, count: Int) {
        if (current is HYComposeRoot) {
            return
        }
        current.move(from, to, count)
    }

    override fun onClear() {
        if (current is HYComposeRoot) {
            return
        }
        current.clear()
    }

}