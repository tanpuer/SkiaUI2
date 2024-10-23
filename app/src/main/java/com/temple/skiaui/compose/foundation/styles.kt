package com.temple.skiaui.compose.foundation

fun Modifier.setSize(width: Int, height: Int): Modifier {
    styles["size"] = intArrayOf(width, height)
    return this
}

fun Modifier.setBackgroundColor(color: String): Modifier {
    styles["backgroundColor"] = color
    return this
}