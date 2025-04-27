package com.temple.skiaui.compose.ui.util

import android.content.res.Resources
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import androidx.annotation.DrawableRes

fun decodeDrawableResource(res: Resources, @DrawableRes resId: Int, reqWidth: Int): Bitmap {
    val options = BitmapFactory.Options().apply {
        inJustDecodeBounds = true
        BitmapFactory.decodeResource(res, resId, this)
        inSampleSize = calculateInSampleSize(this, reqWidth)
        inJustDecodeBounds = false
    }
    return BitmapFactory.decodeResource(res, resId, options)
}

private fun calculateInSampleSize(options: BitmapFactory.Options, reqWidth: Int): Int {
    val width = options.outWidth
    var inSampleSize = 1
    if (width > reqWidth) {
        val halfWidth = width / 2
        while (halfWidth / inSampleSize >= reqWidth) {
            inSampleSize *= 2
        }
    }
    return inSampleSize
}