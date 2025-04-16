package com.temple.skiaui.compose.ui

import android.util.Log
import com.temple.skiaui.compose.foundation.Modifier

class HYComposePage(modifier: Modifier, val onShow: () -> Unit, val onHide: () -> Unit) :
    HYComposeViewGroup(modifier) {

    override fun getViewType(): String = "Page"

    fun push(width: Int) {
        nativePush(ref, width)
    }

    fun pop() {
        nativePop(ref)
    }

    private fun onShowFromNative() {
        Log.d("HYComposePage", "onShow")
        onShow()
    }

    private fun onHideFromNative() {
        Log.d("HYComposePage", "onHide")
        onHide()
    }

    private external fun nativePush(page: Long, width: Int)
    private external fun nativePop(page: Long)

}