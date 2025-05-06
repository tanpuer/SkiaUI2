package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeScrollView(modifier: Modifier) : HYComposeFlexboxLayout(modifier) {

    private var position = -1
    private var offset = 0

    override fun getViewType(): String = "Scroll"

    fun scrollToPosition(position: Int) {
        if (position == this.position) {
            return
        }
        nativeScrollToPosition(ref, position)
    }

    fun scrollTo(offset: Int) {
        if (offset == this.offset) {
            return
        }
        nativeScrollOffset(ref, offset)
    }

    fun getDistanceByIndex(index: Int): Int {
        return nativeGetDistanceByIndex(ref, index)
    }

    private external fun nativeScrollToPosition(ref: Long, position: Int)
    private external fun nativeScrollOffset(ref: Long, offset: Int)
    private external fun nativeGetDistanceByIndex(ref: Long, index: Int): Int

}