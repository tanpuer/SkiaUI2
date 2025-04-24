package com.temple.skiaui.bitmap

import android.graphics.drawable.Drawable

interface ImageLoader : Drawable.Callback {

    fun requestBitmap(source: String)

    fun requestDrawable(resId: Int)

    fun start()

    fun stop()

    fun release()

    override fun scheduleDrawable(who: Drawable, what: Runnable, `when`: Long) {}

    override fun unscheduleDrawable(who: Drawable, what: Runnable) {}

}