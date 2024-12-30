package com.temple.skiaui.platform

import android.graphics.Canvas

interface ICanvasProvider {

    fun lockCanvas(originCanvas: Canvas): Canvas?

    fun unLockCanvas(canvas: Canvas)

}