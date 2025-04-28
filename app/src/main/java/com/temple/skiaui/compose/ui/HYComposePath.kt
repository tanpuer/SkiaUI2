package com.temple.skiaui.compose.ui

class HYComposePath : AutoReleasable {

    var ref = nativeInit()

    fun moveTo(x: Float, y: Float) {
        nativeMoveTo(ref, x, y)
    }

    fun lineTo(x: Float, y: Float) {
        nativeLineTo(ref, x, y)
    }

    fun close() {
        nativeClose(ref)
    }

    override fun release() {
        nativeDeInit(ref)
    }

    private external fun nativeInit(): Long
    private external fun nativeMoveTo(ref: Long, x: Float, y: Float)
    private external fun nativeLineTo(ref: Long, x: Float, y: Float)
    private external fun nativeClose(ref: Long)
    private external fun nativeDeInit(ref: Long)
}