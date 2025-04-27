package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeCanvas(modifier: Modifier) : HYComposeView(modifier), Canvas {

    private var onDrawCallback: (canvas: Canvas) -> Unit = {}

    override fun getViewType(): String = "Canvas"

    private fun onDrawFromJNI() {
        onDrawCallback.invoke(this)
    }

    fun setDrawCallback(callback: (canvas: Canvas) -> Unit) {
        onDrawCallback = callback
    }

    override fun drawRect(l: Float, t: Float, r: Float, b: Float, paint: HYComposePaint) {
        nativeDrawRect(ref, l, t, r, b, paint.ref)
    }

    private external fun nativeDrawRect(
        ref: Long,
        l: Float,
        t: Float,
        r: Float,
        b: Float,
        paint: Long
    )

}