package com.temple.skiaui.compose.ui.util

import androidx.compose.ui.unit.Dp
import com.temple.skiaui.HYSkiaUIApp

private val density = HYSkiaUIApp.getInstance().resources.displayMetrics.density

fun dp2px(dp: Dp) = (dp.value * density + 0.5).toInt()

fun px2dp(px: Int): Dp = Dp(px / density)

fun dpArrayToIntArray(array: Array<Dp>): IntArray {
    return IntArray(array.size) { index ->
        dp2px(array[index])
    }
}