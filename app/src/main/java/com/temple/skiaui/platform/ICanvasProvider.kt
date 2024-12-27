package com.temple.skiaui.platform

import android.graphics.Canvas

interface ICanvasProvider {

    fun lockCanvas(): Canvas?

    fun unLockCanvas(canvas: Canvas)

}