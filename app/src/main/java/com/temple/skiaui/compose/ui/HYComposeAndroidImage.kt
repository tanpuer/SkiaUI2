package com.temple.skiaui.compose.ui

import androidx.annotation.DrawableRes
import com.temple.skiaui.compose.foundation.Modifier

class HYComposeAndroidImage(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType() = "AndroidImage"

    fun setResource(source: String?) {
        if (source.isNullOrEmpty()) {
            return
        }
        nativeSetResource(ref, source)
    }

    fun setResId(@DrawableRes resId: Int) {
        if (resId == 0) {
            return
        }
        nativeSetResId(ref, resId)
    }

    fun setContentScale(scale: ContentScale) {
        if (scale == ContentScale.Unspecified) {
            return
        }
        nativeSetScale(ref, scale.value)
    }

    private external fun nativeSetResource(ref: Long, source: String)
    private external fun nativeSetResId(ref: Long, resId: Int)
    private external fun nativeSetScale(ref: Long, scale: String)

}