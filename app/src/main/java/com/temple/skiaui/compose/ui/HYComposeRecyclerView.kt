package com.temple.skiaui.compose.ui

import androidx.compose.runtime.Composable
import com.temple.skiaui.compose.foundation.Modifier

class HYComposeRecyclerView(
    modifier: Modifier,
    val createView: (index: Int) -> HYComposeView,
    val bindView: (index: Int, node: HYComposeView) -> Unit,
    val viewType: (index: Int) -> Int,
    val dataSize: Int
) :
    HYComposeScrollView(modifier) {

    override fun getViewType(): String = "RecyclerView"

    fun setDataSize(size: Int) {
        nativeSetDataSize(ref, size)
    }

    private fun createViewFromJNI(index: Int): Long {
        return createView(index).ref
    }

    private fun bindViewFromJNI(index: Int, view: HYComposeView) {
        bindView(index, view)
    }

    fun getViewTypeFromJNI(index: Int): Int {
        return viewType(index)
    }

    private external fun nativeSetDataSize(ref: Long, size: Int)

}