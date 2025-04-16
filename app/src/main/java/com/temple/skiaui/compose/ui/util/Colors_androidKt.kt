package com.temple.skiaui.compose.ui.util

import androidx.compose.ui.graphics.Color

internal fun composeColorToSkiaColor(color: Color): Int {
    return (color.value shr 32).toUInt().toInt()
}