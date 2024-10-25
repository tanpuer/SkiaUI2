package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

abstract class HYComposeNode(modifier: Modifier) {

    var ref: Long = -1L

    var parentRef: Long = -1L

    var contextPtr = modifier.context

    val view: HYComposeView by lazy {
        val view = createComposeView()
        initStyles(view, modifier)
        initAttrs(view, modifier)
        view
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

    abstract fun createComposeView(): HYComposeView

    private fun initStyles(view: HYComposeView, modifier: Modifier) {
        modifier.styles.forEach { (key, value) ->
            when (key) {
                "size" -> {
                    val size = value as IntArray
                    view.setSize(value[0], value[1])
                }

                "backgroundColor" -> {
                    view.setBackgroundColor(value as String)
                }

            }
        }
    }

    open fun initAttrs(view: HYComposeView, modifier: Modifier) {

    }

    private external fun nativeAddView(parent: Long, child: Long)

}