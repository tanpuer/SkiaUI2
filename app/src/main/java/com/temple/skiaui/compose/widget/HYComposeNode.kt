package com.temple.skiaui.compose.widget

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
    }

    fun addElement(index: Int, child: Long, parent: Long) {
        parentRef = parent
        nativeAddView(parent, child)
    }

    fun removeElement(index: Int, count: Int) {
//        nativeRemoveViews(ref, index, count)
    }

    fun move(from: Int, to: Int, count: Int) {

    }

    fun clear() {

    }

    abstract fun getViewType(): String

    open fun initStyles(modifier: Modifier) {
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "size" -> {
                    val size = value as IntArray
                    (this as? HYComposeView)?.setSize(size[0], size[1])
                }

                "backgroundColor" -> {
                    (this as? HYComposeView)?.setBackgroundColor(value as String)
                }

            }
        }
    }

    open fun initAttrs(modifier: Modifier) {}

    private external fun nativeAddView(parent: Long, child: Long)
    private external fun nativeCreateView(contextPtr: Long, type: String): Long

}