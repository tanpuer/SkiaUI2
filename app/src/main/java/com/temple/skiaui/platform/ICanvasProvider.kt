package com.temple.skiaui.platform

import android.graphics.Canvas
import android.view.Surface

interface ICanvasProvider {

    fun lockCanvas(originCanvas: Canvas): Canvas?

    fun unLockCanvas(canvas: Canvas)

    fun getSurface(): Surface?

}