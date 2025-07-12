package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

abstract class HYComposeNode(var modifier: Modifier) {

    var ref: Long = 0L

    private var parentRef: Long = 0L

    init {
        initNativeView()
    }

    private fun initNativeView() {
        ref = nativeCreateView(getViewType())
        initAttrs(modifier)
        initEvents(modifier)
    }

    fun addElement(index: Int, child: Long, parent: Long) {
        parentRef = parent
        nativeAddView(parent, child, index)
    }

    fun removeElement(index: Int, count: Int) {
        nativeRemoveViews(ref, index, count)
    }

    fun move(ref: Long, from: Int, to: Int, count: Int) {
        nativeMove(ref, from, to, count)
    }

    open fun clear() {

    }

    abstract fun getViewType(): String

    open fun initAttrs(modifier: Modifier) {}

    open fun initEvents(modifier: Modifier) {}

    fun updateModifier(modifier: Modifier) {
        if (modifier != this.modifier) {
            modifier.diffStyles(this.modifier)
        } else {
            modifier.diffStyles(Modifier())
        }
        innerUpdateModifier(modifier)
        this.modifier = modifier
    }

    open fun innerUpdateModifier(modifier: Modifier) {}

    private external fun nativeAddView(parent: Long, child: Long, index: Int)
    private external fun nativeCreateView(type: String): Long
    private external fun nativeRemoveViews(parent: Long, index: Int, count: Int)
    private external fun nativeMove(parent: Long, from: Int, to: Int, count: Int)
    private external fun nativeRemoveAllChildren()

}