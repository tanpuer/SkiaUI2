package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

abstract class HYComposeNode(val modifier: Modifier) {

    var ref: Long = 0L

    private var parentRef: Long = 0L

    private var contextPtr: Long = 0L

    init {
        this.contextPtr = modifier.context
        initNativeView()
    }

    private fun initNativeView() {
        ref = nativeCreateView(contextPtr, getViewType())
        initStyles(modifier)
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

    }

    fun clear() {

    }

    abstract fun getViewType(): String

    open fun initStyles(modifier: Modifier) {}

    open fun initAttrs(modifier: Modifier) {}

    open fun initEvents(modifier: Modifier) {}

    private external fun nativeAddView(parent: Long, child: Long, index: Int)
    private external fun nativeCreateView(contextPtr: Long, type: String): Long
    private external fun nativeRemoveViews(parent: Long, index: Int, count: Int)
    private external fun nativeMove(parent: Long, from: Int, to: Int, count: Int)
    private external fun nativeRemoveAllChildren()

}