package com.temple.skiaui.compose.foundation

fun Modifier.setSize(width: Int, height: Int): Modifier {
    styles["size"] = intArrayOf(width, height)
    return this
}

fun Modifier.setBackgroundColor(color: String): Modifier {
    styles["backgroundColor"] = color
    return this
}

fun Modifier.setWidth(width: Int): Modifier {
    styles["width"] = width
    return this
}

fun Modifier.setHeight(height: Int): Modifier {
    styles["height"] = height
    return this
}

fun Modifier.setTextSize(size: Int): Modifier {
    styles["textSize"] = size
    return this
}

fun Modifier.setJustifyContent(value: String): Modifier {
    styles["justifyContent"] = value
    return this
}

fun Modifier.setAlignItems(value: String): Modifier {
    styles["alignItems"] = value
    return this
}

fun Modifier.setFlexDirection(value: String): Modifier {
    styles["flexDirection"] = value
    return this
}

fun Modifier.setMargins(margins: IntArray): Modifier {
    styles["margins"] = margins
    return this
}