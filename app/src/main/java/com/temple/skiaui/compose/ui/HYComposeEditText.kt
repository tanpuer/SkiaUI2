package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeEditText(modifier: Modifier, hint: String, focus: Boolean = false) :
    HYComposeView(modifier) {

    fun setHint(hint: String) {
        nativeSetHint(ref, hint)
    }

    fun setFocus(focus: Boolean) {
        nativeSetFocus(ref, focus)
    }

    override fun getViewType(): String = "EditText"

    private external fun nativeSetHint(editText: Long, hint: String)
    private external fun nativeSetFocus(editText: Long, focus: Boolean)

}