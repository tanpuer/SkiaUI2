package com.temple.skiaui.platform

interface HYSurfaceViewCallback {

    fun onSurfaceCreated()

    fun onSurfaceChanged(width: Int, height: Int)

    fun onSurfaceDestroyed()

    fun onShow()

    fun onHide()

}