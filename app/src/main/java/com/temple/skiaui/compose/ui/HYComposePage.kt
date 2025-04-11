package com.temple.skiaui.compose.ui

import android.util.Log
import com.temple.skiaui.compose.foundation.Modifier

class HYComposePage(modifier: Modifier) : HYComposeViewGroup(modifier) {

    override fun getViewType(): String = "Page"

    fun push(width: Int) {
        nativePush(ref, width)
    }

    fun pop() {
        nativePop(ref)
    }

    private fun onShow() {
        Log.d("HYComposePage", "onShow")
    }

    private fun onHide() {
        Log.d("HYComposePage", "onHide")
    }

    private external fun nativePush(page: Long, width: Int)
    private external fun nativePop(page: Long)

}